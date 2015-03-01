varying vec3 N;
varying vec3 v;
varying vec2 varTexCoord;
varying vec2 varTexCoordCloud;
varying float constantAttenuation;
varying float linearAttenuation;
varying float quadraticAttenuation;
uniform mat4  textureMat4;


void main(void){
    //calculate pos vertex in 3D word space
    v = vec3(gl_ModelViewMatrix * gl_Vertex);
    //calculate normal
    N = normalize(gl_NormalMatrix * gl_Normal);
    //return uv cord
    varTexCoord = vec2( textureMat4 * gl_MultiTexCoord0 );
    //calculate attenuation
    constantAttenuation=gl_LightSource[0].constantAttenuation;
    linearAttenuation=gl_LightSource[0].linearAttenuation;
    quadraticAttenuation=gl_LightSource[0].quadraticAttenuation;
     //calculate vertex pos
     gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
