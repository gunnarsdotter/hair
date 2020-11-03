// hair shader

#version 150

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vec2 texCoord[3];   //[3] because this makes a triangle
in vec3 exNormal[3];
out vec2 texCoordG;
out vec3 exNormalG;

void main()
{
  float hairLength = 0.6; 
  for(int i = 0; i < gl_in.length(); i++)
  {
    //create a start/root point
    gl_Position = gl_in[i].gl_Position; 
    texCoordG = texCoord[i];
    exNormalG = exNormal[i];
    EmitVertex(); //create vertex

    //create a vertex in normalsdir from pos
    gl_Position =  gl_in[i].gl_Position + vec4(exNormal[i], 1)* hairLength; 
    EmitVertex();
    //End and create a new
    EndPrimitive(); 
  }
  
  
}