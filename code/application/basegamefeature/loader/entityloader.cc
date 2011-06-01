//------------------------------------------------------------------------------
//  loader/entityloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "loader/entityloader.h"
#include "game/entity.h"
#include "managers/factorymanager.h"
#include "managers/entitymanager.h"
#include "managers/categorymanager.h"
#include "basegameattr/basegameattributes.h"
#include "graphicsattr/graphicsattributes.h"
#include "loader/loaderserver.h"
#include "properties/cameraproperty.h"

namespace BaseGameFeature
{
using namespace Game;

__ImplementClass(EntityLoader, 'LOEL', EntityLoaderBase);

//------------------------------------------------------------------------------
/**
*/
bool
EntityLoader::Load(const Util::Array<Util::String>& activeLayers)
{
    CategoryManager* categoryManager = CategoryManager::Instance();
    FactoryManager* factoryManager = FactoryManager::Instance();
    const Util::String treeCategory("Tree");
    const Util::String envCategory("_Environment");

    // flag that we are loading
    EntityLoaderBase::insideLoading = true;
    // the category manager has already loaded the instance tables
    // for this level, so we don't need to do a direct database
    // access...
    IndexT catIndex;
    SizeT numCategories = categoryManager->GetNumCategories();
    for (catIndex = 0; catIndex < numCategories; catIndex++)
    {
        const CategoryManager::Category& category = categoryManager->GetCategoryByIndex(catIndex);
        if (category.HasInstanceDataset())
        {
            // check for special categories that are handled by other loaders
            if ((category.GetName() != treeCategory) && (category.GetName() != envCategory))
            {
                // get the instance table
                Db::ValueTable* table = category.GetInstanceDataset()->Values();
                IndexT rowIndex;
                SizeT numRows = table->GetNumRows();
                for (rowIndex = 0; rowIndex < numRows; rowIndex++)
                {
                    // only load entity if part of an active layer
                    if (this->EntityIsInActiveLayer(table, rowIndex, activeLayers))
                    {
                        // create entity through factory manager
                        Ptr<Entity> gameEntity = factoryManager->CreateEntityByCategory(category.GetName(), table, rowIndex);
                        // update progress indicator
                        this->UpdateProgressIndicator(gameEntity);

                        // attach the entity to the world
                        EntityManager::Instance()->AttachEntity(gameEntity); 
                    }
                }
            }
        }
    }
    EntityLoaderBase::insideLoading = false;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
EntityLoader::UpdateProgressIndicator(const Ptr<Game::Entity>& gameEntity)
{
    n_assert(gameEntity);
    //BaseGameFeature::LoaderServer* loaderServer = BaseGameFeature::LoaderServer::Instance();

    Util::String id;
    if (gameEntity->HasAttr(Attr::Id))
    {
        id = gameEntity->GetString(Attr::Id);
    }
    Util::String graphics;
    if (gameEntity->HasAttr(Attr::Graphics))
    {
        graphics = gameEntity->GetString(Attr::Graphics);
    }
    //Util::String progressText;
    //progressText.Format("Loading game entity '%s' (gfx=%s)...", id.Get(), graphics.AsCharPtr()());
    //loaderServer->SetProgressText(progressText);
    //loaderServer->AdvanceProgress(1);
    //loaderServer->UpdateProgressDisplay();
}

}; // namespace BaseGameFeature


