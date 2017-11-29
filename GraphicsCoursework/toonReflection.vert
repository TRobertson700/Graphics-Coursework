// toon shader.vert
#version 330

uniform mat4 modelview;
uniform mat4 modelMatrix;
uniform mat4 projection;
uniform vec4 lightPosition;
uniform vec3 cameraPos;

layout(location = 3) in vec2 in_TexCoord;

in  vec3 in_Position;
in  vec3 in_Normal;
out vec3 ex_N;
out vec3 ex_V;
out vec3 ex_L;
out float ex_D;

out vec3 ex_WorldNorm;
out vec3 ex_WorldView;
out vec2 ex_TexCoord;

// multiply each vertex position by the MVP matrix
// and find V, L, N vectors for the fragment shader
void main(void) {

	// vertex into eye coordinates
	vec4 vertexPosition = modelview * vec4(in_Position,1.0);
	float ex_D = distance(vertexPosition,lightPosition);//Distance from light to vertex
	
	// Find V - in eye coordinates, eye is at (0,0,0)
	ex_V = normalize(-vertexPosition).xyz;

	// surface normal in eye coordinates
	// taking the rotation part of the modelview matrix to generate the normal matrix
	// (if scaling is includes, should use transpose inverse modelview matrix!)
	// this is somewhat wasteful in compute time and should really be part of the cpu program,
	// giving an additional uniform input
	mat3 normalmatrix = transpose(inverse(mat3(modelview)));
	ex_N = normalize(normalmatrix * in_Normal);

	// L - to light source from vertex
	ex_L = normalize(lightPosition.xyz - vertexPosition.xyz);

	vec3 worldPos = (modelMatrix * vec4(in_Position, 1.0)).xyz;
	mat3 normalWorldMatrix = transpose(inverse(mat3(modelMatrix)));

	ex_WorldNorm = normalWorldMatrix * in_Normal;
	ex_WorldView = cameraPos - worldPos;

	ex_TexCoord = in_TexCoord;
    gl_Position = projection * vertexPosition;
}