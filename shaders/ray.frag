#version 410 core

in vec3 pos;

out vec4 fragColor;


uniform vec2 viewport;
uniform mat4 scale;
uniform mat4 view;

uniform int NUM_TRIS = 1;
uniform int NUM_OBJECTS = 1;
uniform int NUM_LIGHTS = 1;

// to be uniforms or blocks
vec3 colors[10];
mat4 invs[10];
uniform vec3 lightPos[10];

const float INF = 10000.0;
const float EPS = 0.00001;

const int TRI_SIZE = 4096;

//vec3 tris[10];
layout (std140) uniform triBlock {
    vec3 tris[TRI_SIZE];
//    vec3 trisz[TRI_SIZE];
};




///////////////////////////////////////////
//              INTERSECTIONS            //
///////////////////////////////////////////

int findT(in float a, in float b, in float c, out float t1, out float t2)
{
    // intersections, intersectDist1, intersectDist2
    vec3 t = vec3(0.0, INF, INF);

    if (abs(a) < EPS)
    {
        t1 = -c / b;
        return 1;
    }

    float disc = b * b - 4.0 * a * c;

    // one solution
    if (abs(disc) < EPS)
    {
        t1 = -b / (2.0 * a);
        t.x = 0.0;
        return 1;
    }

    // no solutions
    if (disc < 0.0)
        return 0;

    // two solutions (disc > 0)
    t1 = (-b + sqrt(disc)) / (2.0 * a);
    t2 = (-b - sqrt(disc)) / (2.0 * a);
    return 2;
}

////////////////////SPHERE/////////////////////

// assumes radius size 1.f
vec4 intersectSphere(in vec4 p, in vec4 d)
{
    vec4 n = vec4(0.0, 0.0, 0.0, INF);

    vec4 v;
    float t1, t2;
    float a = d.x * d.x + d.y * d.y + d.z * d.z;
    float b = 2.0 * p.x * d.x + 2.0 * p.y * d.y + 2.0 * p.z * d.z;
    float c = p.x * p.x + p.y * p.y + p.z * p.z - 1;
    int tees = findT(a, b, c, t1, t2);

    if (tees > 0)
    {
        v = p + d * t1;
        if (t1 < 0.0)
            t1 = INF;
        if (t1 < n.w)
        {
            v.w = t1;
            n = v;
        }
        if (tees > 1)
        {
            v = p + d * t2;
            if (t2 < 0.0)
                t2 = INF;
            if (t2 < n.w)
            {
                v.w = t2;
                n = v;
            }
        }
    }
    return n;
}

////////////////////TRIANGLE/////////////////////

vec4 intersectTriangle(in vec4 p, in vec4 d, in int index)
{
    vec3 v1 = tris[index];
    vec3 v2 = tris[index+1];
    vec3 v3 = tris[index+2];
    vec4 n = vec4(cross(v2 - v1, v3 - v1), INF);
    if (dot(d, n) > 0.0)
        return n;

    // intersect infinit plane
    n.w = dot(-n.xyz, p.xyz - v1) / dot(n.xyz, d.xyz);
    if (n.w < 0.0)
        return vec4(n.xyz, INF);

    vec3 point = p.xyz + d.xyz * n.w;

    // check if collision point is within triangle
    vec3 pab = cross(v1 - point, v2 - point);
    vec3 pbc = cross(v2 - point, v3 - point);
    vec3 pca = cross(v3 - point, v1 - point);

    if (dot(pab, pbc) < EPS || dot(pbc, pca) < EPS)
        n.w = INF;

    return n;
}


vec3 calcColor(in int index, in vec3 point, in vec3 normal, in vec3 eye)
{
    return colors[index];
}


vec3 raytrace(in vec4 p, in vec4 d, in int depth)
{
    vec3 normal;
    int colorIndex;
    float bestT = INF;

    vec4 n;

    for (int i = 0; i < NUM_TRIS; ++i)
    {
        n = intersectTriangle(p, d, i * 3);
        if (n.w < bestT)
        {
            bestT = n.w;
            normal = n.xyz;
            colorIndex = 0;
        }
    }

    for (int i = 0; i < NUM_OBJECTS; ++i)
    {
        vec4 p_shape = invs[i] * p;
        vec4 d_shape = invs[i] * d;
        n = intersectSphere(p_shape, d_shape);
        if (n.w < bestT)
        {
            bestT = n.w;
            normal = n.xyz;
            colorIndex = i + 1;
        }
    }

    if (bestT < INF)
    {
        vec4 point = p + d * n.w;
        return calcColor(colorIndex, point.xyz, n.xyz, p.xyz);
    }

    return vec3(0);
}




void main(){

    // testing
//    tris[0] = vec3(0,1,-5);
//    tris[1] = vec3(-1,-1,-5);
//    tris[2] = vec3(1,-1,-3);

    colors[0] = vec3(0, 1, 0);
    colors[1] = vec3(1);

    invs[0] = mat4(2, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 2, 0,
                   0, 0, 10, 1);
    // end testing

    mat4 ftw = inverse(scale * view);
    vec4 eye = inverse(view) * vec4(0, 0, 0, 1);
    vec4 farWorld = ftw * vec4(pos, 1);
    vec4 dir = normalize(farWorld - eye);

    vec3 color = raytrace(eye, dir, 0);
    fragColor = vec4(color, 1);
}
