varying vec4 shadowCoord;
uniform mat4 delphMVP;
 mat4 biasMat=mat4(  0.5, 0.0, 0.0, 0.0,
                     0.0, 0.5, 0.0, 0.0,
                     0.0, 0.0, 0.5, 0.0,
                     0.5, 0.5, 0.5, 1.0 );

void main(){
	//ModelViewProjectionMatrix * vtx
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	//shadow coord biasMat*delphMVP * vtx
	shadowCoord=(biasMat*delphMVP)*gl_Vertex;
} 