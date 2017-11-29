// toon fragment shader 
#version 330

// Some drivers require the following
precision highp float;

struct lightStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct materialStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};


uniform lightStruct light;
uniform materialStruct material;
uniform sampler2D textureUnit0;
uniform samplerCube textureUnit1;

uniform float attConst;
uniform float attLinear;
uniform float attQuadratic;

in vec3 ex_WorldNorm;
in vec3 ex_WorldView;

in float ex_D;
in vec3 ex_N;
in vec3 ex_V;
in vec3 ex_L;
in vec2 ex_TexCoord;

layout(location = 0) out vec4 out_Color;
 
void main(void) {
    
	// Ambient intensity
	vec4 ambientI = light.ambient * material.ambient;

	// Diffuse intensity
	vec4 diffuseI = light.diffuse * material.diffuse;
	diffuseI = diffuseI * max(dot(normalize(ex_N),normalize(ex_L)),0);

	// Specular intensity
	// Calculate R - reflection of light
	vec3 R = normalize(reflect(normalize(-ex_L),normalize(ex_N)));

	vec4 specularI = light.specular * material.specular;
	specularI = specularI * pow(max(dot(R,ex_V),0), material.shininess);

	// Fragment colour	
	float attenuation=1.0f/(attConst + attLinear * ex_D + attQuadratic * ex_D*ex_D);
	vec4 tmp_Color = (diffuseI + specularI);
	//Attenuation does not affect transparency
	vec4 litColour = vec4(tmp_Color.rgb *attenuation, tmp_Color.a);
	vec4 amb=min(ambientI,vec4(1.0f));


	vec4 shade1 = smoothstep(vec4(0.2f), vec4(0.21f), litColour);
	vec4 shade2 = smoothstep(vec4(0.4f), vec4(0.41f), litColour);
	vec4 shade3 = smoothstep(vec4(0.8f), vec4(0.81f), litColour);
	
	float metallic = dot(ex_N, ex_V);
	metallic = smoothstep(0.4,0.6,metallic);
	metallic = metallic/2 + 0.75;
	
	vec4 colourA = max( max(0.3*shade1,0.5*shade2), shade3);

	if ( abs(dot(ex_N,ex_V)) < 0.25)
	{
		colourA = vec4(vec3(0.0),1.0) * metallic;
	}

	vec3 texColor = texture(textureUnit0, ex_TexCoord).rgb;
	vec3 reflectTexCoord = reflect(-ex_WorldView, normalize(ex_WorldNorm));

	//vec4 texel = colourA * metallic * vec4(texColor, 1.0);
	vec4 texel = min(colourA * metallic * vec4(texColor, 1.0) * texture(textureUnit1, reflectTexCoord), vec4(1.0f));

	out_Color.rgb = texel.rgb;
	out_Color.a = 1.0;
}