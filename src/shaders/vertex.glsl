
layout(location=0) in vec3 modelSpace_vertices;
layout(location = 1) in vec3 vertex_colors;

uniform mat4 MVP;
out vec3 v_colors;
void main()
{
	v_colors = vertex_colors;
	gl_Position = MVP * vec4(modelSpace_vertices,1.0);
}