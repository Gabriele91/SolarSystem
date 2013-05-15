
varying vec4 vertColor;
varying vec4 vertTexcoord;

void main(){
	vertTexcoord = gl_MultiTexCoord0;  
    vertColor=gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
} 
