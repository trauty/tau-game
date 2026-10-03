import tau_surface;
import util;

// Tier 1 with a twist: triplanar sampling and an alpha cutoff, and still no lighting code. The
// engine owns the entry points; this only has to answer "what is the surface here, and is there
// one at all".
[ShaderConfig("Surface", "Cutout", true, true, true)]
struct CutoutLitShader : ISurfaceMaterial
{
    float4 base_color;

    uint albedo_tex;
    uint albedo_sampler;

    float roughness;
    float metallic;
    float alpha_cutoff;

    property Texture albedo { get { return {albedo_tex, albedo_sampler}; } }

    bool surface(VertexOut input, out Surface out_surface, out float out_alpha)
    {
        float4 sampled =
            sampleTriplanar(albedo.tex2d(), albedo.samplerState(), input.world_pos, input.normal, 10.0f) * base_color;

        out_surface.albedo = sampled.rgb;
        out_surface.normal = input.normal;
        out_surface.roughness = roughness;
        out_surface.metallic = metallic;
        out_alpha = sampled.a;

        return sampled.a >= alpha_cutoff;
    }
};

[shader("vertex")]
VertexOut vertexMain(uint vertex_id: SV_VertexID, uint instance_id: SV_InstanceID,
                     uint base_instance: SV_StartInstanceLocation)
{ return defaultVertexTransform(vertex_id, instance_id, base_instance); }

[shader("fragment")]
SurfaceOutput fragmentMain(VertexOut input) { return tauSurfaceFragment<CutoutLitShader>(input); }

[shader("fragment")]
GBufferOutput gbufferMain(VertexOut input) { return tauGBufferFragment<CutoutLitShader>(input); }
