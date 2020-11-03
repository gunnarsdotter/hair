#version 150

//in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;


uniform mat4 matrix;
uniform mat4 rotation1;
uniform mat4 rotation2;
uniform mat4 m1;
uniform mat4 m2;

uniform vec3 position1;
uniform vec3 position2;

out vec4 g_color;


// Uppgift 3: Soft-skinning på GPU
//
// Flytta över din implementation av soft skinning från CPU-sidan
// till vertexshadern. Mer info finns på hemsidan.

void main(void)
{	
	//beräkna matriserna T*R*T-1
	/*mat4 T1 = mat4( 
			1.0, 0.0, 0.0, position1.x,
			0.0, 1.0, 0.0, position1.y,
			0.0, 0.0, 1.0, position1.z,
			0.0, 0.0, 0.0, 1.0);
	mat4 T2 = mat4( 
			1.0, 0.0, 0.0, position2.x,
			0.0, 1.0, 0.0, position2.y,
			0.0, 0.0, 1.0, position2.z,
			0.0, 0.0, 0.0, 1.0);
	
				
	mat4 M1 = T1 * rotation1 * inverse(T1) ;
	mat4 M2 = T2 * rotation2 * inverse(T2) ;
	*/
	const vec3 lightDir = normalize(vec3(0.3, 0.5, 1.0));
	// transformera resultatet med ModelView- och Projection-matriserna
	gl_Position = matrix * vec4(in_Position, 1.0);
	//gl_Position = matrix*(m1*in_TexCoord.x  + m2*in_TexCoord.y)*vec4(in_Position, 1.0);


	// sätt röd+grön färgkanal till vertex Weights
	vec4 color = vec4(in_TexCoord.x, in_TexCoord.y, 0.0, 1.0);

	// Lägg på en enkel ljussättning på vertexarna 	
	float intensity = dot(in_Normal, lightDir);
	color.xyz *= intensity;

	g_color = color;
}

