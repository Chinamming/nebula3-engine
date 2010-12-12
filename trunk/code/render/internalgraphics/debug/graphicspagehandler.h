#pragma once
//------------------------------------------------------------------------------
/**
    @class Debug::GraphicsPageHandler
    
    Provides debug information about (internal)graphics subsystem.
    
    (C) 2008 Radon Labs GmbH
*/
#include "http/httprequesthandler.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "internalgraphics/internalmodelentity.h"
#include "internalgraphics/internalcameraentity.h"
#include "http/html/htmlpagewriter.h"
#include "resources/resourceid.h"
#include "lighting/internalabstractlightentity.h"
#include "models/modelinstance.h"
#include "models/nodes/transformnodeinstance.h"
#include "models/nodes/animatornodeinstance.h"
#include "models/nodes/statenodeinstance.h"
#include "models/nodes/shapenodeinstance.h"

//------------------------------------------------------------------------------
namespace Debug
{
class GraphicsPageHandler : public Http::HttpRequestHandler
{
    __DeclareClass(GraphicsPageHandler);
public:
    /// constructor
    GraphicsPageHandler();
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void HandleRequest(const Ptr<Http::HttpRequest>& request);

private:
    /// lookup a character model entity by numeric id
    Ptr<InternalGraphics::InternalModelEntity> LookupCharacterModelEntityById(InternalGraphics::InternalGraphicsEntity::Id id);
    /// write a table with graphics entities
    void WriteEntityTable(const Util::Array<Ptr<InternalGraphics::InternalGraphicsEntity> >& entities, const Ptr<Http::HtmlPageWriter>& htmlWriter);
    /// write information page for a single stage
    Http::HttpStatus::Code WriteStageInfo(const Ptr<Http::HttpRequest>& request, const Resources::ResourceId& stageName);
    /// write information page for a single view
    Http::HttpStatus::Code WriteViewInfo(const Ptr<Http::HttpRequest>& request, const Resources::ResourceId& viewName);
    /// write information page for a single entity
    Http::HttpStatus::Code WriteEntityInfo(const Ptr<Http::HttpRequest>& request, InternalGraphics::InternalGraphicsEntity::Id entityId);
    /// write information for a model entity
    void WriteModelEntityInfo(const Ptr<InternalGraphics::InternalModelEntity>& modelEntity, const Ptr<Http::HtmlPageWriter>& htmlWriter);
    /// write information for a light entity
    void WriteLightEntityInfo(const Ptr<Lighting::InternalAbstractLightEntity>& lightEntity, const Ptr<Http::HtmlPageWriter>& htmlWriter);
    /// write information for a camera entity
    void WriteCameraEntityInfo(const Ptr<InternalGraphics::InternalCameraEntity>& cameraEntity, const Ptr<Http::HtmlPageWriter>& htmlWriter);
    /// play an animation on a character
    Http::HttpStatus::Code PlayAnim(const Ptr<Http::HttpRequest>& request, const Util::String& argString);
    /// toggle character skin visibility
    Http::HttpStatus::Code ToggleSkin(const Ptr<Http::HttpRequest>& request, const Util::String& argString);
    /// apply character skin list
    Http::HttpStatus::Code ApplySkinList(const Ptr<Http::HttpRequest>& request, const Util::String& argString);
    /// apply character variation
    Http::HttpStatus::Code ApplyVariation(const Ptr<Http::HttpRequest>& request, const Util::String& argString);
    /// write model node hierarchy
    void WriteModelInstanceHierarchy(int entityId, const Ptr<Models::ModelInstance>& modelInstance, const Ptr<Http::HtmlPageWriter>& htmlWriter);
    /// write model node instance  info
    Http::HttpStatus::Code WriteModelNodeInstanceInfo(const Ptr<Http::HttpRequest>& request, const Util::String& argString);
    /// write transform node instance info                                            
    void WriteTransformNodeInfo(const Ptr<Models::TransformNodeInstance>& transformNode, const Ptr<Http::HtmlPageWriter>& htmlWriter);
    /// write animator node instance info                                            
    void WriteAnimatorNodeInfo(const Ptr<Models::AnimatorNodeInstance>& animNode, const Ptr<Http::HtmlPageWriter>& htmlWriter);
    /// write state node instance info                                            
    void WriteStateNodeInfo(const Ptr<Models::StateNodeInstance>& stateNode, const Ptr<Http::HtmlPageWriter>& htmlWriter);
    /// write shapenode instance info                                            
    void WriteShapeNodeInfo(const Ptr<Models::ShapeNodeInstance>& shapeNode, const Ptr<Http::HtmlPageWriter>& htmlWriter);
    /// show or hide the AnimSequencer debug hud
    Http::HttpStatus::Code ShowHideAnimSequencerHUD(const Ptr<Http::HttpRequest>& request, const Util::String& argString);
};

} // namespace Debug
//------------------------------------------------------------------------------
    