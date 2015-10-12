#version 140
vec3 position;
vec2 texIn;
vec3 normal;

out vec2 texCoor;
out vec4 v_position; // position of the vertex (and fragment) in world space
smooth out vec3 _NormalDirection;  // surface normal vector in world space

uniform mat4  mP, mV, mM;   // transformation matrices
uniform mat3 m_3x3_inv_transp;

void main() {
  v_position = mM * vec4(position, 1.0);
  _NormalDirection = normalize(m_3x3_inv_transp * normal);

  mat4 mvp = mP*mV*mM;
  gl_Position = mvp * vec4(position, 1.0);
  texCoor = texIn;
}