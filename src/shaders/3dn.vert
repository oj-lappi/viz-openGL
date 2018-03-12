uniform mat4 MV;
uniform mat4 TR;
uniform mat4 ROT;

attribute vec3 vCol;
attribute vec3 vPos;
attribute vec3 vNorm;
varying vec3 color;
void main()
{
gl_Position = MV*ROT*TR *vec4(vPos,1.0);
color = vCol;
}
