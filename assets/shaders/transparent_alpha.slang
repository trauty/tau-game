import smol_surface;

// Transparent geometry stays in the forward pass, which is exactly why Tier 1 still works for it:
// the material describes a surface, the engine lights it, and the blend mode decides which pass
// it lands in. Nothing here names a pass.
[ShaderConfig("Surface", "TransparentAlpha", false, true, false)]
struct TransparentAlphaShader : ISurfaceMaterial
{
    float4 base_color;

    uint albedo_tex;
    uint albedo_sampler;

    float roughness;
    float metallic;

    property Texture albedo { get { return {albedo_tex, albedo_sampler}; } }

    bool surface(VertexOut input, out Surface out_surface, out float out_alpha)
    {
        float4 sampled = albedo.sample(input.uv) * base_color;

        out_surface.albedo = sampled.rgb;
        out_surface.normal = input.normal;
        out_surface.roughness = roughness;
        out_surface.metallic = metallic;
        out_alpha = sampled.a;
        return true;
    }
};

[shader("vertex")]
VertexOut vertexMain(uint vertex_id: SV_VertexID, uint instance_id: SV_InstanceID,
                     uint base_instance: SV_StartInstanceLocation)
{ return defaultVertexTransform(vertex_id, instance_id, base_instance); }

[shader("fragment")]
SurfaceOutput fragmentMain(VertexOut input) { return smolSurfaceFragment<TransparentAlphaShader>(input); }
