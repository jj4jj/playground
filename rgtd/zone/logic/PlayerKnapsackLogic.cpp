
#include "PlayerKnapsackLogic.h"


//todo

db::RolePackage * PlayerKnapsackLogic::GetPackInfo()
{
    return player->MutableRoleData()->mutable_pack();
}

void    PlayerKnapsackLogic::OnInitBirthPlayer()
{
    db::RolePackage * pack = GetPackInfo();
    pack->set_maxgrid(10);    
    //pack->set_nextgid(1);    
}
void    PlayerKnapsackLogic::OnEvent(int iEvent,uint64_t ulParam ,void * arg )
{

}

//get role item
int   PlayerKnapsackLogic::CreateItem(db::RoleItem & item)
{
    db::RolePackage * pack = GetPackInfo();
    uint32_t nextgid = pack->nextgid();
    item.set_gid(nextgid);
    ++nextgid;
    pack->set_nextgid(nextgid);
    return 0;
}
bool     PlayerKnapsackLogic::IsGridFill(uint32_t pos)
{
    db::RolePackage * pack = GetPackInfo();
    if(pos >= pack->maxgrid())
    {
        return true;
    }

    int ngrids = pack->grids_size();      
    for(int i = 0;i < ngrids ;++i)
    {
        if(pack->grids(i).pos() == pos &&
           pack->grids(i).num() > 0 )
        {
            return true;
        }
    }  
    return false;    
}
int PlayerKnapsackLogic::GetFreeGridPos()
{
    int pos = 1;
    db::RolePackage * pack = GetPackInfo();
    while(pos < (int)pack->maxgrid() && 
          IsGridFill(pos) )
    {
        ++pos;
    }
    if(pos >= (int)pack->maxgrid())
    {
        return -1;
    }
    return pos;
}
//role item    
int   PlayerKnapsackLogic::PutItem(const db::RoleItem & item,uint32_t num)
{
    db::RolePackage * pack = GetPackInfo();
    int iCap = GetItemCapacity(item.id());
    if(iCap < (int)num)
    {
        return -1;
    }
    int ngrids = pack->grids_size();
    //todo read from cfg    
    int32_t iMaxTiled = 1;
    //get
    assert(iMaxTiled > 0);
    int iOneFree = 0,iOneFill = 0;
    for(int i = 0;i < ngrids ;++i)
    {
        db::RolePackGrid & one_grid = *(pack->mutable_grids(i));
        db::RoleItem & one_item = *one_grid.mutable_item();
        iOneFill = one_grid.num();
        iOneFree = iMaxTiled - iOneFill;
        if(item.id() == one_item.id() &&
           iOneFree > 0)
        {
            if((int)num > iOneFree)
            {
                num -= iOneFree;    
                one_grid.set_num(iMaxTiled);
            }
            else
            {
                iOneFill += num;
                num = 0;
                one_grid.set_num(iOneFill);
            }
        }
    }
    while( num > 0)
    {
        db::RolePackGrid * grid = pack->add_grids();
        db::RoleItem newitem = item;
        CreateItem(newitem);
        *(grid->mutable_item()) = newitem;
        //grid
        grid->set_pos(GetFreeGridPos());
        
        if(iMaxTiled > (int)num)
        {
            grid->set_num(num);
            num = 0;
        }
        else
        {
            grid->set_num(iMaxTiled);
            num -= iMaxTiled;
        }
    }   

    //event add
    uint64_t param = item.id();
    param <<= 32;
    param |= num;
    EventCenter::Instance().FireEvent(EVENT_PLAYER_ITEM_ADD,NULL,0,param);        
    return 0;
}
int   PlayerKnapsackLogic::GetItemCapacity(uint32_t  id)
{
    db::RolePackage * pack = GetPackInfo();

    int ngrids = pack->grids_size();
    //todo read from cfg    
    int32_t iMaxTiled = 1;
    //get
    assert(iMaxTiled > 0);
    int iNum = 0;
    int iOneFree = 0,iOneFill = 0;
    for(int i = 0;i < ngrids ;++i)
    {
        db::RolePackGrid & one_grid = *(pack->mutable_grids(i));
        db::RoleItem & one_item = *one_grid.mutable_item();
        iOneFill = one_grid.num();
        iOneFree = iMaxTiled - iOneFill;
        if(id == one_item.id() &&
           iOneFree > 0)
        {
            iNum += iOneFree;
        }
    }
    int nEmptyGrids = pack->maxgrid() - ngrids;
    assert(nEmptyGrids >= 0);
    iNum += (nEmptyGrids * iMaxTiled);
    return iNum;
}
int   PlayerKnapsackLogic::RemoveItem(uint32_t id,uint32_t num)
{
    if(GetItemNum(id) < (int)num)
    {
        return -1;
    }
    db::RolePackage * pack = GetPackInfo();
    int ngrids = pack->grids_size();
    //todo read from cfg    
    int32_t iMaxTiled = 1;
    //get
    assert(iMaxTiled > 0);
    int iOneItemNum = 0;
    std::vector<uint32_t>   rmposvec;
    for(int i = 0;i < ngrids && num > 0;++i)
    {
        db::RolePackGrid & one_grid = *(pack->mutable_grids(i));
        db::RoleItem & one_item = *one_grid.mutable_item();
        iOneItemNum = one_grid.num();
        if(id == one_item.id() &&
           iOneItemNum > 0)
        {
            if(iOneItemNum >= (int)num)
            {
                iOneItemNum -= num;
                num = 0;                
            }
            else
            {
                num -= iOneItemNum;
                iOneItemNum = 0;
            }
            one_grid.set_num(iOneItemNum);
            if(0 == iOneItemNum )
            {
                rmposvec.push_back(one_grid.pos());
            }
        }
    }
    ////////////////////////////////////////////////
    for(uint i = 0;i < rmposvec.size(); ++i)
    {
        RemoveGrid(rmposvec[i]);
    }

    //event add
    uint64_t param = id;
    param <<= 32;
    param |= num;
    EventCenter::Instance().FireEvent(EVENT_PLAYER_ITEM_DEC,NULL,0,param);        
    return 0;    
}
db::RolePackGrid *  PlayerKnapsackLogic::FindPackGrid(uint32_t pos)
{
    db::RolePackage * pack = GetPackInfo();
    int idx = FindPackGridIdx(pos);
    if(idx < 0 )
    {
        return NULL;
    }
    return pack->mutable_grids(idx);
}
int PlayerKnapsackLogic::FindPackGridIdx(uint32_t pos)
{
    db::RolePackage * pack = GetPackInfo();
    if(pos >= pack->maxgrid())
    {
        return -1;
    }
    int ngrids = pack->grids_size();
    for(int i = 0;i < ngrids ;++i)
    {
        if(pack->grids(i).pos() == pos)
        {
            return i;
        }
    }
    return -1;
}
int   PlayerKnapsackLogic::RemoveGrid(uint32_t pos)
{
    int idx =  FindPackGridIdx(pos);
    if(idx < 0)
    {
        return -1;
    }
    db::RolePackage * pack = GetPackInfo();
    int size =  pack->grids_size();
    pack->mutable_grids()->SwapElements(idx,size-1);
    pack->mutable_grids()->RemoveLast();
    ///////////////////////////////////////////////
    EventCenter::Instance().FireEvent(EVENT_PLAYER_REMOVE_GRID_ITEM,NULL,0,pos);
    return 0;
}

int   PlayerKnapsackLogic::GetItemNum(uint32_t id)
{
    db::RolePackage * pack = GetPackInfo();

    int ngrids = pack->grids_size();
    //todo read from cfg    
    int iNum = 0;
    for(int i = 0;i < ngrids ;++i)
    {
        db::RolePackGrid & one_grid = *(pack->mutable_grids(i));
        db::RoleItem & one_item = *one_grid.mutable_item();
        if(id == one_item.id() &&
           one_grid.num() > 0)
        {
            iNum += one_grid.num();
        }
    }
    return iNum;
}
int   PlayerKnapsackLogic::GetMaxGrid()
{
    db::RolePackage * pack = GetPackInfo();
    return pack->maxgrid();
}
int   PlayerKnapsackLogic::ChangeMaxGrid(uint8_t chg)
{
    db::RolePackage * pack = GetPackInfo();
    int old = GetMaxGrid();
    old += chg;
    pack->set_maxgrid(old);
    EventCenter::Instance().FireEvent(EVENT_PLAYER_CHANGE_PACK_MAX_GRID,NULL,0,old);
    return 0;
}


