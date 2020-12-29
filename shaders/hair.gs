#version 410 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

in vec3 teNormal[3];
out vec3 exNormalG;

uniform mat4 projMatrix;
//Global params
    float mass = 0.0007;
    float hairRadius = 0.008; 
    float hairLength = 0.009; 

// Calculat the offset from vertex with the crossproduct
float calculateOffset(int i){
    vec4 cam = normalize(gl_in[i].gl_Position - vec4(0.0, 0.0, -3.5, 0.0));
    vec3 ofs = normalize(cross(teNormal[i], cam.xyz))*hairRadius;
    return ofs;
}
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void leftHair(int i,  int j, float offset){
    gl_Position =  projMatrix*(
                                gl_in[i].gl_Position +rand(vec2(i,j))*0.05
                              + vec4(normalize(teNormal[i])*hairLength, 0.0)*j 
                              + vec4(0.0, -9.82, 0.0, 1.0)*mass*j
                              );
    exNormalG = teNormal[i];
    EmitVertex();
}
void rightHair(int i, int j, float offset){
    gl_Position =  projMatrix*(
                                gl_in[i].gl_Position
                              + offset +rand(vec2(i,j))*0.05
                              + vec4(normalize(teNormal[i])*hairLength, 0.0)*j
                              + vec4(0.0, -9.82, 0.0, 1.0)*mass*j
                              ); 
    exNormalG = teNormal[i];
    EmitVertex();
}

//Create two triangles as a squre 
//change depending on the right-hand rule. 
void createHair(int i, float offset){
    
     for(int j = 0; j < 10; j++)
      { 
        if(i%2 == 0){
             rightHair(i,  j,  offset);
             leftHair( i,  j,  offset);
             j++;
             leftHair( i,  j,  offset);
             rightHair(i,  j,  offset);
         }
         else{
             leftHair( i,  j,  offset);
             rightHair(i,  j,  offset);
             j++;
             rightHair(i,  j,  offset);
             leftHair( i,  j,  offset);
         }
    }
}   

void main()
{
    for(int i = 0; i < gl_in.length(); i++)
    {
        float offset = calculateOffset(i);
        createHair(i, offset);
        EndPrimitive(); 
    } 

}