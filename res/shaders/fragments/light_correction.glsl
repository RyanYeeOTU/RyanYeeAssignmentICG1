#pragma once
#include "frame_uniforms.glsl"


vec3 lightCorrection(vec3 inputLight) {
    // set lightinput when flag is enabled
    if (IsFlagSet(FLAG_ENABLE_LIGHTS))
    {
       return inputLight;
    }
    // Otherwise just return the input
    else 
    {
        return vec3(0,0,0);
    }
}

