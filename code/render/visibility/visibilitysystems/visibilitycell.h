#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::VisibilityCell
  
    Hierarchies of VisibilityCell objects group graphics entities by spatial 
    relationship. They are the key class for efficient visibility
    queries. An application may derive specialized subclasses of VisibilityCell
    which must only adhere to the following 2 simple rules:
    
    - if the VisibilityCell object is fully visible, all child VisibilityCells and 
      all Entities attached to the VisibilityCell are guaranteed to be visible
    - if the VisibilityCell object is fully invisible, all child VisibilityCells and
      all Entities attached to the VisibilityCell are guaranteed to be invisible

      @todo: need to handle extra shadow bounding box
      @todo: statistics and profiling
      @todo: need to add visibility depending on LOD
      @todo: multithreaded visibility link update?
      @todo: add dirty handling to visibility links (e.g. don't need to
             update links between static lights and static objects)


    (C) 2010 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "math/bbox.h"
#include "visibility/visibilitycontext.h"
#include "visibility/observercontext.h"
#include "internalgraphics/internalgraphicsentitytype.h"

//------------------------------------------------------------------------------
namespace Visibility
{

class VisibilityCell : public Core::RefCounted
{
    __DeclareClass(VisibilityCell);
public:
    /// constructor
    VisibilityCell();
    /// destructor
    virtual ~VisibilityCell();

    /// called when attached to visibility system
    virtual void OnAttach();
    /// called when removed visibility system
    virtual void OnRemove();

    /// set the VisibilityCell's world space bounding box
    void SetBoundingBox(const Math::bbox& box);
    /// get the VisibilityCell's world space transform
    const Math::bbox& GetBoundingBox() const;
        
    /// add a child cell (only during setup phase)
    void AttachChildCell(const Ptr<VisibilityCell>& cell);
    /// get pointer to parent cell (returns invalid pointer if this is root cell)
    const Ptr<VisibilityCell>& GetParentCell() const;
    /// get current child cells
    const Util::Array<Ptr<VisibilityCell> >& GetChildCells() const;

    /// attach a graphics context to this VisibilityCell
    void AttachContext(const Ptr<VisibilityContext>& context);
    /// remove a graphics context from this VisibilityCell
    void RemoveContext(const Ptr<VisibilityContext>& context);
    /// insert an context into the VisibilityCell hierarchy, return found child cell
    Ptr<VisibilityCell> InsertContext(const Ptr<VisibilityContext>& context);
    /// get all entities
    const Util::Array<Ptr<VisibilityContext> >& GetEntityContexts() const;
    /// get entities by entity type
    const Util::Array<Ptr<VisibilityContext> >& GetEntitiesByType(InternalGraphics::InternalGraphicsEntityType::Code type) const;
    /// get the number of entities in hierarchy
    SizeT GetNumEntitiesInHierarchy() const;
    /// get the number of entities in hierarchy by type
    SizeT GetNumEntitiesInHierarchyByType(InternalGraphics::InternalGraphicsEntityType::Code t) const;
    /// get the number of entities in hierarchy by entity type mask
    SizeT GetNumEntitiesInHierarchyByTypeMask(uint entityTypeMask) const;

    /// recursively collect all visible context
    void CollectVisibleContexts(const Ptr<ObserverContext>& observerContext, Util::Array<Ptr<VisibilityContext> >& visibilityContexts, uint entityTypeMask);
    /// starting from this cell, find smallest containment cell in cell tree
    Ptr<VisibilityCell> FindEntityContainmentCell(const Ptr<VisibilityContext>& entity);

private:
    friend class VisibilityContext;
         
    /// create links between visible entities
    void RecurseCollectVisibleContexts(const Ptr<ObserverContext>& observerContext, Util::Array<Ptr<VisibilityContext> >& visibilityContexts, uint entityTypeMask, Math::ClipStatus::Type clipStatus);
    /// increment/decrement the numEntitiesInHierarchy counter (including in all parent cells)
    void UpdateNumEntitiesInHierarchy(InternalGraphics::InternalGraphicsEntityType::Code type, int num);

    Ptr<VisibilityCell> parentCell;
    SizeT numEntitiesInHierarchyAllTypes;
    SizeT numEntitiesInHierarchyByType[InternalGraphics::InternalGraphicsEntityType::NumTypes];
    Math::bbox boundingBox;
    Util::Array<Ptr<VisibilityCell> > childCells;
    Util::Array<Ptr<VisibilityContext> > entities;
    Util::Array<Ptr<VisibilityContext> > entitiesByType[InternalGraphics::InternalGraphicsEntityType::NumTypes];
};

//------------------------------------------------------------------------------
/**
*/
inline void
VisibilityCell::SetBoundingBox(const Math::bbox& box)
{
    this->boundingBox = box;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
VisibilityCell::GetBoundingBox() const
{
    return this->boundingBox;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<VisibilityCell>& 
VisibilityCell::GetParentCell() const
{
    return this->parentCell;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<VisibilityCell> >& 
VisibilityCell::GetChildCells() const
{
    return this->childCells;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<VisibilityContext> >&
VisibilityCell::GetEntityContexts() const
{
    return this->entities;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<VisibilityContext> >&
VisibilityCell::GetEntitiesByType(InternalGraphics::InternalGraphicsEntityType::Code type) const
{
    n_assert((type >= 0) && (type < InternalGraphics::InternalGraphicsEntityType::NumTypes));
    return this->entitiesByType[type];
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
VisibilityCell::GetNumEntitiesInHierarchy() const
{
    return this->numEntitiesInHierarchyAllTypes;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
VisibilityCell::GetNumEntitiesInHierarchyByType(InternalGraphics::InternalGraphicsEntityType::Code type) const
{
    return this->numEntitiesInHierarchyByType[type];
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
VisibilityCell::GetNumEntitiesInHierarchyByTypeMask(uint entityTypeMask) const
{
    SizeT numEntities = 0;
    IndexT entityType;
    for (entityType = 0; entityType < InternalGraphics::InternalGraphicsEntityType::NumTypes; entityType++)
    {
        if (0 != (entityTypeMask & (1<<entityType)))
        {
            numEntities += this->numEntitiesInHierarchyByType[entityType];
        }
    }
    return numEntities;
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------
