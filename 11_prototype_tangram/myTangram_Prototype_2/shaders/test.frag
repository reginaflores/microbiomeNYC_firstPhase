#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;

vec2 tile(vec2 _st, float _zoom){
    _st *= _zoom;
    return fract(_st);
}

float box(vec2 _st, vec2 _size, float _smoothEdges){
    _size = vec2(0.5)-_size*0.5;
    vec2 aa = vec2(_smoothEdges*0.5);
    vec2 uv = smoothstep(_size,_size+aa,_st);
    uv *= smoothstep(_size,_size+aa,vec2(1.0)-_st);
    return uv.x*uv.y;
}

void main(void){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = vec3(0.0);

    // Divide the space in 18
    // st = tile(st,18.);   
    st *= 18.;
    st = fract(st);
    st += sin(u_time*0.5);

    // Draw a Go Board
    float smoothEdges = 0.01;
    vec2 size = vec2(0.9);
    size = vec2(0.5)-size*0.5;

    vec2 aa = vec2(smoothEdges*0.5);
    vec2 uv2 = smoothstep(size,size+aa,st);
    uv2 *= smoothstep(size,size+aa,vec2(1.0)-st);

    color = vec3(uv2.x*uv2.y);
   
    
    gl_FragColor = vec4(color,1.0);    

}


