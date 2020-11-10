// hair shader

#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

in vec2 texCoord[3];   //[3] because this makes a triangle
in vec3 exNormal[3];
out vec2 texCoordG;
out vec3 exNormalG;
uniform mat4 projMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 worldToViewMatrix;

float calculateOffset(int i, float hairRadius){
    vec4 cam = normalize(gl_in[i].gl_Position - vec4(0.0, 0.0, -3.5, 0.0));
    vec3 ofs = normalize(cross(exNormal[i], cam.xyz))*hairRadius;
    return ofs;
}
void leftHair(int i,  int j, float offset, float hairLength){
    //create a vertex in normalsdir from pos
    gl_Position =  projMatrix*((gl_in[i].gl_Position) + vec4(normalize(exNormal[i])*hairLength, 0.0)*j); 
    texCoordG = texCoord[i];
    exNormalG = exNormal[i];
    EmitVertex();
}
void rightHair(int i, int j, float offset, float hairLength){
    //create a vertex in normalsdir from pos
    gl_Position =  projMatrix*((gl_in[i].gl_Position + offset ) + vec4(normalize(exNormal[i])*hairLength, 0.0)*j); 
    texCoordG = texCoord[i];
    exNormalG = exNormal[i];
    EmitVertex();
}
void createHair(int i, float offset, float hairLength){
     for(int j = 0; j < 3; j++)
      { 
        if(i%2 == 0){
             rightHair(i,  j,  offset,  hairLength);
             leftHair( i,  j,  offset,  hairLength);
             j++;
             leftHair( i,  j,  offset,  hairLength);
             rightHair(i,  j,  offset,  hairLength);
         }
         else{
             leftHair( i,  j,  offset,  hairLength);
             rightHair(i,  j,  offset,  hairLength);
             j++;
             rightHair(i,  j,  offset,  hairLength);
             leftHair( i,  j,  offset,  hairLength);
         }
    }
}   
void main()
{
  float hairRadius = 0.005; 
  float hairLength = 0.05; 
  for(int i = 0; i < gl_in.length(); i++)
  {
    float offset = calculateOffset(i, hairRadius);
    createHair(i, offset, hairLength);
    //End and create a new
    EndPrimitive(); 
  } 
}