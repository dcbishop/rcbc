varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;
uniform sampler2D tex;

void main()
{
	vec3 n,halfV;
	float NdotL,NdotHV;
	
	/* The ambient term will always be present */
	vec4 frag_color = ambient;
	
	/* a fragment shader can't write a varying variable, hence we need
	a new variable to store the normalized interpolated normal */
	n = normalize(normal);
	
	/* compute the dot product between normal and ldir */
	NdotL = max(dot(n,lightDir),0.0);

	if (NdotL > 0.0) {
		frag_color += diffuse * NdotL;
		halfV = normalize(halfVector);
		NdotHV = max(dot(n,halfV),0.0);
		frag_color += gl_FrontMaterial.specular * 
				gl_LightSource[0].specular * 
				pow(NdotHV, gl_FrontMaterial.shininess);
	}
	float fc = gl_FragCoord.z;
	float test = clamp(gl_FragCoord.z, 2.0, 3.0);
	//vec4 final = {clamp(gl_FragCoord.z, 0.0, 1.0), fract(gl_FragCoord.y), sin(gl_FragCoord.z), 0.5};
	vec4 final = {test, test, test, 1.0f};
	vec4 texture_color = texture2D(tex,gl_TexCoord[0].st);
	//texture_color.a = 0.5;
	gl_FragColor = texture_color;
}
