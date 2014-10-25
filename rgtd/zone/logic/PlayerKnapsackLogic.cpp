
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
    uint32_t nextgid = 1;//todo get from data
    //item->set_gid(nextgid);
    ++nextgid;
    //set next gid
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
        if(pack->grids(i)->pos() == pos &&
           pack->grids(i)->num() > 0 )
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
    while(pos < pack->maxgrid() && 
          IsGridFill(pos) )
    {
        ++pos;
    }
    if(pos >= pack->maxgrid())
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
    if(iCap < num)
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
        db::RoleItem & one_item = one_grid->mutable_item();
        iOneFill = one_item.num();
        iOneFree = iMaxTiled - iOneFill;
        if(item.id() == one_item.id() &&
           iOneFree > 0)
        {
            if(num > iOneFree)
            {
                num -= iOneFree;    
                one_item.set_num(iMaxTiled);
            }
            else
            {
                iOneFill += num;
                num = 0;
                one_item.set_num(iOneFill);
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
        grid->set_grid(GetFreeGridPos());
        
        if(iMaxTiled > num)
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
        db::RoleItem & one_item = one_grid->mutable_item();
        iOneFill = one_item.num();
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
        db::RoleItem & one_item = one_grid->mutable_item();
        iOneItemNum = one_item.num();
        if(id == one_item.id() &&
           iOneItemNum > 0)
        {
            if(iOneItemNum >= num)
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
    for(int i = 0;i < ngrids && num > 0;++i)
    {
        if(pack->grids(i)->pos() == pos)
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
        db::RoleItem & one_item = one_grid->mutable_item();
        if(id == one_item.id() &&
           one_item.num() > 0)
        {
            iNum += one_item.num();
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
}


