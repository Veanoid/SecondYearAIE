// a simple flat colour shader
#version 440

in vec2 vTexCoord;
in vec3 vNormal;
in vec4 vPosition;

uniform vec3 Ka; // ambient material colour 
uniform sampler2D Kd; // diffuse material colour
uniform vec3 Ks; // specual material colour
uniform float specularPower; // material specular power

uniform vec3 Ia; // ambient light colour

uniform vec3 Id; // diffuse light colour
uniform vec3 Is; // specular light colour
uniform vec3 LightDirection;

out vec4 FragColour;

uniform vec3 cameraPosition;

void main() 
{

	// ensure normal and ligth direction are normalised
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);

	// calculate lambert term (negate light direction)
	float lambertTerm = max( 0, min( 1, dot( N, -L)));

	// calculate view vector and reflection vector
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	// calculate specular term
	float specularTerm = pow( max( 0, dot( R, V )), specularPower );

	// calculate each colour property
	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * texture(Kd, vTexCoord).xyz * lambertTerm;
	vec3 specular = Is * Ks * specularTerm;

	FragColour = vec4( ambient + diffuse + specular, 1 );
}