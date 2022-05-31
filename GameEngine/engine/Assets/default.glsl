
#if defined(VERTEX)

layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTexCoord;

uniform mat4 ObjectTransform;
uniform mat4 ObjectProjection;

out vec2 vTexCoord;

void main()
{
	vTexCoord = aTexCoord;

	gl_Position =ObjectProjection* ObjectTransform * vec4(aPos, 1.0);
}

#elif defined(FRAGMENT)

	
out vec4 FragColor;

uniform vec4 ourColor;
uniform int hasimage;
uniform sampler2D image;

in vec3 vertexcolor;
in vec2 vTexCoord;

void main()
{
	//FragColor = vec4(vertexcolor,1.0);
	//FragColor = ourColor;
	if(hasimage == 1)
		FragColor = texture(image,vTexCoord);
	else
		FragColor = ourColor;
	//FragColor = vec4(vTexCoord, 0.0, 0.0);
}

#endif