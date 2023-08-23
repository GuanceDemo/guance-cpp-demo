// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/exporters/otlp/otlp_grpc_exporter_factory.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/provider.h"
#include "opentelemetry/sdk/resource/semantic_conventions.h"

// sdk::TracerProvider is just used to call ForceFlush and prevent to cancel running exportings when
// destroy and shutdown exporters.It's optional to users.
#include "opentelemetry/sdk/trace/tracer_provider.h"

#include "foo_library/foo_library.h"

namespace trace     = opentelemetry::trace;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace otlp      = opentelemetry::exporter::otlp;
namespace resource = opentelemetry::sdk::resource;

namespace
{
    opentelemetry::exporter::otlp::OtlpGrpcExporterOptions opts;
    void InitTracer()
    {
        // Create OTLP exporter instance
        auto exporter  = otlp::OtlpGrpcExporterFactory::Create(opts);
        auto processor = trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));

        resource::ResourceAttributes attributes = {
                {resource::SemanticConventions::kServiceName, "guance-cpp-otel-grpc-exporter-demo"}
        };
        auto resource = opentelemetry::sdk::resource::Resource::Create(attributes);

        std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
                trace_sdk::TracerProviderFactory::Create(std::move(processor), std::move(resource));
        // Set the global trace provider
        trace::Provider::SetTracerProvider(provider);
    }

    void CleanupTracer()
    {
        // We call ForceFlush to prevent to cancel running exportings, It's optional.
        opentelemetry::nostd::shared_ptr<opentelemetry::trace::TracerProvider> provider =
                trace::Provider::GetTracerProvider();
        if (provider)
        {
            static_cast<trace_sdk::TracerProvider *>(provider.get())->ForceFlush();
        }

        std::shared_ptr<opentelemetry::trace::TracerProvider> none;
        trace::Provider::SetTracerProvider(none);
    }
}  // namespace

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        opts.endpoint = argv[1];
        if (argc > 2)
        {
            opts.use_ssl_credentials         = true;
            opts.ssl_credentials_cacert_path = argv[2];
        }
    }
    // Removing this line will leave the default noop TracerProvider in place.
    InitTracer();

    foo_library();

    CleanupTracer();
}