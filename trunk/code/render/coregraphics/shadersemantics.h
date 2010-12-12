#pragma once
//------------------------------------------------------------------------------
/**
    @file coregraphics/shadersemantics.h
    
    Standard shader variable semantic names.
    
    (C) 2009 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
// hrmpf, Cg converts all semantics to upper-case...
#if __PS3__
#define NEBULA3_SEMANTIC_MODELVIEWPROJECTION        "MODELVIEWPROJECTION"
#define NEBULA3_SEMANTIC_MODEL                      "MODEL"
#define NEBULA3_SEMANTIC_VIEW                       "VIEW"
#define NEBULA3_SEMANTIC_MODELVIEW                  "MODELVIEW"
#define NEBULA3_SEMANTIC_INVMODELVIEW               "INVMODELVIEW"
#define NEBULA3_SEMANTIC_INVVIEW                    "INVVIEW"
#define NEBULA3_SEMANTIC_VIEWPROJECTION             "VIEWPROJECTION"
#define NEBULA3_SEMANTIC_EYEPOS                     "EYEPOS"
#define NEBULA3_SEMANTIC_FOCALLENGTH                "FOCALLENGTH"
#define NEBULA3_SEMANTIC_INVPROJECTION              "INVPROJECTION"
#define NEBULA3_SEMANTIC_PROJECTION                 "PROJECTION"
#define NEBULA3_SEMANTIC_HALFPIXELSIZE              "HALFPIXELSIZE"
#define NEBULA3_SEMANTIC_PIXELSIZE                  "PIXELSIZE"
#define NEBULA3_SEMANTIC_CHARACTERINDEX             "CHARACTERINDEX"
#define NEBULA3_SEMANTIC_JOINTPALETTE               "JOINTPALETTE"
#define NEBULA3_SEMANTIC_DEBUGSHADERLAYER           "DEBUGSHADERLAYER"
#define NEBULA3_SEMANTIC_TIME                       "TIME"
#define NEBULA3_SEMANTIC_EMITTERTRANSFORM           "EMITTERTRANSFORM"
#define NEBULA3_SEMANTIC_BILLBOARD                  "BILLBOARD"
#define NEBULA3_SEMANTIC_FADEVALUE                  "FADEVALUE"
#define NEBULA3_SEMANTIC_SATURATION                 "SATURATION"
#define NEBULA3_SEMANTIC_BALANCE                    "BALANCE"
#define NEBULA3_SEMANTIC_FOGCOLOR                   "FOGCOLOR"
#define NEBULA3_SEMANTIC_FOGDISTANCES               "FOGDISTANCES"
#define NEBULA3_SEMANTIC_HDRBLOOMCOLOR              "HDRBLOOMCOLOR"
#define NEBULA3_SEMANTIC_HDRBLOOMSCALE              "HDRBLOOMSCALE"
#define NEBULA3_SEMANTIC_HDRBRIGHTPASSTHRESHOLD     "HDRBRIGHTPASSTHRESHOLD"
#define NEBULA3_SEMANTIC_DOFDISTANCES               "DOFDISTANCES"
#define NEBULA3_SEMANTIC_DIFFMAP0                   "DIFFMAP0"
#define NEBULA3_SEMANTIC_DIFFMAP1                   "DIFFMAP1"
#define NEBULA3_SEMANTIC_INTENSITY0                 "INTENSITY0"
#define NEBULA3_SEMANTIC_INTENSITY1                 "INTENSITY1"
#define NEBULA3_SEMANTIC_INTENSITY2                 "INTENSITY2"
#define NEBULA3_SEMANTIC_LIGHTPOSRANGE              "LIGHTPOSRANGE"
#define NEBULA3_SEMANTIC_LIGHTCOLOR                 "LIGHTCOLOR"
#define NEBULA3_SEMANTIC_NORMALDEPTHBUFFER          "NORMALDEPTHBUFFER"
#define NEBULA3_SEMANTIC_LIGHTBUFFER                "LIGHTBUFFER"
#define NEBULA3_SEMANTIC_GLOBALLIGHTDIR             "GLOBALLIGHTDIR"
#define NEBULA3_SEMANTIC_GLOBALLIGHTCOLOR           "GLOBALLIGHTCOLOR"
#define NEBULA3_SEMANTIC_GLOBALAMBIENTLIGHTCOLOR    "GLOBALAMBIENTLIGHTCOLOR"
#define NEBULA3_SEMANTIC_GLOBALBACKLIGHTCOLOR       "GLOBALBACKLIGHTCOLOR"
#define NEBULA3_SEMANTIC_GLOBALBACKLIGHTOFFSET      "GLOBALBACKLIGHTOFFSET"
#define NEBULA3_SEMANTIC_LIGHTPROJMAP               "LIGHTPROJMAP"
#define NEBULA3_SEMANTIC_LIGHTPROJTRANSFORM         "LIGHTPROJTRANSFORM"    
#define NEBULA3_SEMANTIC_SHADOWTRANSFORM            "SHADOWTRANSFORM"
#define NEBULA3_SEMANTIC_SHADOWPROJTRANSFORM        "SHADOWPROJTRANSFORM"
#define NEBULA3_SEMANTIC_SHADOWPROJMAP              "SHADOWPROJMAP"
#define NEBUlA3_SEMANTIC_INVERSELIGHTPROJECTION     "INVLIGHTPROJ"
#define NEBUlA3_SEMANTIC_OBJECTID                   "OBJECTID"
#define NEBUlA3_SEMANTIC_OCCLUSIONCONSTANTS         "OCCLUSIONCONSTANTS"   
#define NEBUlA3_SEMANTIC_SHADOWBUFFERSIZE           "SHADOWBUFFERSIZE"
#define NEBUlA3_SEMANTIC_SHADOWOFFSETSCALE          "SHADOWOFFSETSCALE"
#define NEBUlA3_SEMANTIC_SHADOWCONSTANTS            "SHADOWCONSTANTS"
#define NEBUlA3_SEMANTIC_SHADOWINTENSITY            "SHADOWINTENSITY"
#define NEBULA3_SEMANTIC_BBOXCENTER                 "BBOXCENTER"
#define NEBULA3_SEMANTIC_BBOXSIZE                   "BBOXSIZE"
#else
#define NEBULA3_SEMANTIC_MODELVIEWPROJECTION        "ModelViewProjection"
#define NEBULA3_SEMANTIC_MODEL                      "Model"
#define NEBULA3_SEMANTIC_VIEW                       "View"
#define NEBULA3_SEMANTIC_MODELVIEW                  "ModelView"  
#define NEBULA3_SEMANTIC_INVMODELVIEW               "InvModelView"
#define NEBULA3_SEMANTIC_INVVIEW                    "InvView"
#define NEBULA3_SEMANTIC_VIEWPROJECTION             "ViewProjection"
#define NEBULA3_SEMANTIC_EYEPOS                     "EyePos"
#define NEBULA3_SEMANTIC_FOCALLENGTH                "FocalLength"
#define NEBULA3_SEMANTIC_PROJECTION                 "Projection"
#define NEBULA3_SEMANTIC_INVPROJECTION              "InvProjection"
#define NEBULA3_SEMANTIC_HALFPIXELSIZE              "HalfPixelSize"
#define NEBULA3_SEMANTIC_PIXELSIZE                  "PixelSize"
#define NEBULA3_SEMANTIC_CHARACTERINDEX             "CharacterIndex"
#define NEBULA3_SEMANTIC_JOINTPALETTE               "JointPalette"
#define NEBULA3_SEMANTIC_DEBUGSHADERLAYER           "DebugShaderLayer"
#define NEBULA3_SEMANTIC_TIME                       "Time"
#define NEBULA3_SEMANTIC_EMITTERTRANSFORM           "EmitterTransform"
#define NEBULA3_SEMANTIC_BILLBOARD                  "Billboard"
#define NEBULA3_SEMANTIC_FADEVALUE                  "FadeValue"
#define NEBULA3_SEMANTIC_SATURATION                 "Saturation"
#define NEBULA3_SEMANTIC_BALANCE                    "Balance"
#define NEBULA3_SEMANTIC_FOGCOLOR                   "FogColor"
#define NEBULA3_SEMANTIC_FOGDISTANCES               "FogDistances"
#define NEBULA3_SEMANTIC_HDRBLOOMCOLOR              "HdrBloomColor"
#define NEBULA3_SEMANTIC_HDRBLOOMSCALE              "HdrBloomScale"
#define NEBULA3_SEMANTIC_HDRBRIGHTPASSTHRESHOLD     "HdrBrightPassThreshold"
#define NEBULA3_SEMANTIC_DOFDISTANCES               "DoFDistances"
#define NEBULA3_SEMANTIC_DIFFMAP0                   "DiffMap0"
#define NEBULA3_SEMANTIC_DIFFMAP1                   "DiffMap1"
#define NEBULA3_SEMANTIC_INTENSITY0                 "Intensity0"
#define NEBULA3_SEMANTIC_INTENSITY1                 "Intensity1"
#define NEBULA3_SEMANTIC_INTENSITY2                 "Intensity2"     
#define NEBULA3_SEMANTIC_LIGHTPOSRANGE              "LightPosRange"
#define NEBULA3_SEMANTIC_LIGHTCOLOR                 "LightColor"
#define NEBULA3_SEMANTIC_NORMALBUFFER               "NormalBuffer"
#define NEBULA3_SEMANTIC_DSFOBJECTDEPTHBUFFER       "DSFObjectDepthBuffer"
#define NEBULA3_SEMANTIC_LIGHTBUFFER                "LightBuffer"
#define NEBULA3_SEMANTIC_GLOBALLIGHTDIR             "GlobalLightDir"
#define NEBULA3_SEMANTIC_GLOBALLIGHTCOLOR           "GlobalLightColor"
#define NEBULA3_SEMANTIC_GLOBALAMBIENTLIGHTCOLOR    "GlobalAmbientLightColor"
#define NEBULA3_SEMANTIC_GLOBALBACKLIGHTCOLOR       "GlobalBackLightColor"
#define NEBULA3_SEMANTIC_GLOBALBACKLIGHTOFFSET      "GlobalBackLightOffset"   
#define NEBULA3_SEMANTIC_LIGHTPROJMAP               "LightProjMap"  
#define NEBULA3_SEMANTIC_LIGHTPROJTRANSFORM         "LightProjTransform"   
#define NEBULA3_SEMANTIC_SHADOWTRANSFORM            "ShadowTransform"
#define NEBULA3_SEMANTIC_SHADOWPROJTRANSFORM        "ShadowProjTransform"
#define NEBULA3_SEMANTIC_SHADOWPROJMAP              "ShadowProjMap"
#define NEBUlA3_SEMANTIC_INVERSELIGHTPROJECTION     "InvLightProj"
#define NEBUlA3_SEMANTIC_OBJECTID                   "ObjectId"
#define NEBUlA3_SEMANTIC_OCCLUSIONCONSTANTS         "OcclusionConstants"
#define NEBUlA3_SEMANTIC_SHADOWBUFFERSIZE           "ShadowBufferSize"
#define NEBUlA3_SEMANTIC_SHADOWOFFSETSCALE          "ShadowOffsetScale"
#define NEBUlA3_SEMANTIC_SHADOWCONSTANTS            "ShadowConstants"
#define NEBUlA3_SEMANTIC_SHADOWINTENSITY            "ShadowIntensity"
#define NEBULA3_SEMANTIC_BBOXCENTER                 "BBoxCenter"
#define NEBULA3_SEMANTIC_BBOXSIZE                   "BBoxSize"
#endif
//------------------------------------------------------------------------------
    