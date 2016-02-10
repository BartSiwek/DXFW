struct VertexShaderInput {
  float4 Position : POSITION0;
  float4 Color : COLOR0;
};

struct VertexShaderOutput {
  float4 Position : SV_POSITION;
  float4 Color : COLOR;
};

VertexShaderOutput VS(VertexShaderInput input) {
  VertexShaderOutput output;

  output.Position = input.Position;
  output.Color = input.Color;

  return output;
}

float4 PS(VertexShaderOutput input) : SV_TARGET {
  return input.Color;
}