#include <ctype.h>

#include <Singular/mod2.h>
#include <Singular/ipid.h>
#include <Singular/blackbox.h>
#include <Singular/lists.h>
#include <Singular/ipid.h>
#include <Singular/ipshell.h>
#include <Singular/newstruct.h>

struct newstruct_member_s;
typedef struct newstruct_member_s *newstruct_member;
struct  newstruct_member_s
{
  newstruct_member next;
  char *         name;
  int            typ;
  int            pos;
};

struct newstruct_desc_s
{
  newstruct_member member;
  int            size; // number of mebers +1
};


char * newstruct_String(blackbox *b, void *d)
{
  if (d==NULL) return omStrDup("oo");
  else
  {
    newstruct_desc ad=(newstruct_desc)b->data;
    lists l=(lists)d;
    newstruct_member a=ad->member;
    StringSetS("");
    loop
    {
      StringAppendS(a->name);
      char *tmp=omStrDup(StringAppendS("="));
      if ((!RingDependend(a->typ))
      || (l->m[a->pos].data==NULL)
      || ((l->m[a->pos-1].data==(void *)currRing)
         && (currRing!=NULL)))
      {
        StringSetS("");
        char *tmp2=omStrDup(l->m[a->pos].String());
        StringSetS(tmp);
        StringAppendS(tmp2);
        omFree(tmp2);
      }
      else StringAppendS("??");
      omFree(tmp);
      if (a->next==NULL) break;
      StringAppendS("\n");
      if(errorreported) break;
      a=a->next;
    }
    return omStrDup(StringAppendS(""));
  }
}
lists lCopy_newstruct(lists L)
{
  lists N=(lists)omAlloc0Bin(slists_bin);
  int n=L->nr;
  ring save_ring=currRing;
  N->Init(n+1);
  for(;n>=0;n--)
  {
    if (RingDependend(L->m[n].rtyp)
    && (L->m[n].data!=NULL))
    {
      assume(L->m[n-1].rtyp==RING_CMD);
      if((L->m[n-1].data!=NULL)&&(L->m[n-1].data!=(void*)currRing))
        rChangeCurrRing((ring)(L->m[n-1].data));
      N->m[n].Copy(&L->m[n]);
    }
    else
      N->m[n].Copy(&L->m[n]);
  }
  if (currRing!=save_ring) rChangeCurrRing(save_ring);
  return N;
}
void * newstruct_Copy(blackbox*b, void *d)
{
  lists n1=(lists)d;
  return (void*)lCopy_newstruct(n1);
}

BOOLEAN newstruct_Assign(leftv l, leftv r)
{
  blackbox *ll=getBlackboxStuff(l->Typ());
  if (r->Typ()>MAX_TOK)
  {
    blackbox *rr=getBlackboxStuff(r->Typ());
    if (l->Typ()==r->Typ())
    {
      if (l->Data()!=NULL)
      {
        lists n1=(lists)l->Data();
        n1->Clean(); n1=NULL;
      }
      lists n2=(lists)r->Data();
      n2=lCopy_newstruct(n2);
      if (l->rtyp==IDHDL)
      {
        IDDATA((idhdl)l->data)=(char *)n2;
      }
      else
      {
        l->data=(void *)n2;
      }
      return FALSE;
    }
  }
  Werror("assign %s(%d) = %s(%d)",
        Tok2Cmdname(l->Typ()),l->Typ(),Tok2Cmdname(r->Typ()),r->Typ());
  return TRUE;
}

BOOLEAN newstruct_Op2(int op, leftv res, leftv a1, leftv a2)
{
  // interpreter: a1 is newstruct
  blackbox *a=getBlackboxStuff(a1->Typ());
  newstruct_desc nt=(newstruct_desc)a->data;
  lists al=(lists)a1->Data();
  switch(op)
  {
    case '.':
    {
      if (a2->name!=NULL)
      {
        newstruct_member nm=nt->member;
        while ((nm!=NULL)&&(strcmp(nm->name,a2->name)!=0)) nm=nm->next;
        if (nm==NULL)
        {
          Werror("member %s nor found");
          return TRUE;
        }
        if (RingDependend(nm->typ))
        {
          if (al->m[nm->pos].data==NULL)
	  {
	    // NULL belongs to any ring
	    ring r=(ring)al->m[nm->pos-1].data;
	    if (r!=NULL)
	    {
	      r->ref--;
	      al->m[nm->pos-1].data=NULL;
	      al->m[nm->pos-1].rtyp=DEF_CMD;
	    }
	  }
          else
          {
            //Print("checking ring at pos %d for dat at pos %d\n",nm->pos-1,nm->pos);
            if ((al->m[nm->pos-1].data!=(void *)currRing)
            &&(al->m[nm->pos-1].data!=(void*)0L))
            {
              Werror("different ring %lx(data) - %lx(basering)", al->m[nm->pos-1].data,currRing);
              return TRUE;
            }
          }
          if ((currRing!=NULL)&&(al->m[nm->pos-1].data==NULL))
	  {
	    // remember the ring, if not already set
	    al->m[nm->pos-1].data=(void *)currRing;
	    al->m[nm->pos-1].rtyp=RING_CMD;
	    currRing->ref++;
	  }
        }
        Subexpr r=(Subexpr)omAlloc0Bin(sSubexpr_bin);
        r->start = nm->pos+1;
        memcpy(res,a1,sizeof(sleftv));
        memset(a1,0,sizeof(sleftv));
        if (res->e==NULL) res->e=r;
        else
        {
          Subexpr sh=res->e;
          while (sh->next != NULL) sh=sh->next;
          sh->next=r;
        }
        return FALSE;
      }
      else
      {
        WerrorS("name expected");
        return TRUE;
      }
    }
  }
  return blackboxDefaultOp2(op,res,a1,a2);
}

// BOOLEAN opM(int op, leftv res, leftv args)
BOOLEAN newstruct_OpM(int op, leftv res, leftv args)
{
  // interpreter: args->1. arg is newstruct
  blackbox *a=getBlackboxStuff(args->Typ());
  switch(op)
  {
    case STRING_CMD:
    {
      res->data=(void *)a->blackbox_String(a,args->Data());
      res->rtyp=STRING_CMD;
      return FALSE;
    }
    default:
      Werror("op %d not implemented for type %d",op,args->Typ());
      break;
  }
  return TRUE;
}

void newstruct_destroy(blackbox *b, void *d)
{
  if (d!=NULL)
  {
    lists n=(lists)d;
    n->Clean();
  }
}

void *newstruct_Init(blackbox *b)
{
  newstruct_desc n=(newstruct_desc)b->data;
  lists l=(lists)omAlloc0Bin(slists_bin);
  l->Init(n->size);
  newstruct_member nm=n->member;
  while (nm!=NULL)
  {
    l->m[nm->pos].rtyp=nm->typ;
    l->m[nm->pos].data=idrecDataInit(nm->typ);
    nm=nm->next;
  }
  return l;
}

BOOLEAN newstruct_Check(blackbox *b, void *d)
{
  newstruct_desc n=(newstruct_desc)b->data;
  lists l=(lists)d;
  newstruct_member nm=n->member;
  while (nm!=NULL)
  {
    if ((l->m[nm->pos].rtyp!=nm->typ)
    &&( nm->typ!=DEF_CMD))
    {
      Werror("type change in member %s (%s(%d) -> %s(%d))",nm->name,
          Tok2Cmdname(nm->typ),nm->typ,
	  Tok2Cmdname(l->m[nm->pos].rtyp),l->m[nm->pos].rtyp);
      return TRUE;
    }
    nm=nm->next;
  }
  return FALSE;
}

void newstruct_setup(const char *n, newstruct_desc d )
{
  blackbox *b=(blackbox*)omAlloc0(sizeof(blackbox));
  // all undefined entries will be set to default in setBlackboxStuff
  // the default Print is quite usefule,
  // all other are simply error messages
  b->blackbox_destroy=newstruct_destroy;
  b->blackbox_String=newstruct_String;
  //b->blackbox_Print=blackbox_default_Print;
  b->blackbox_Init=newstruct_Init;
  b->blackbox_Copy=newstruct_Copy;
  b->blackbox_Assign=newstruct_Assign;
  //b->blackbox_Op1=blackboxDefaultOp1;
  b->blackbox_Op2=newstruct_Op2;
  //b->blackbox_Op3=blackbox_default_Op3;
  b->blackbox_OpM=newstruct_OpM;
  b->blackbox_Check=newstruct_Check;
  b->data=d;
  b->properties=1; // list_like
  int rt=setBlackboxStuff(b,n);
  Print("create type %d (%s)\n",rt,n);
}

newstruct_desc newstructFromString(const char *s)
{
  char *ss=omStrDup(s);
  char *p=ss;
  char *start;
  newstruct_desc res=(newstruct_desc)omAlloc0(sizeof(*res));
  res->size=0;
  int t;
  char c;
  newstruct_member elem;
  idhdl save_ring=currRingHdl;
  currRingHdl=(idhdl)1; // fake ring detection

  loop
  {
    // read type:
    while (*p==' ') p++;
    start=p;
    while (isalpha(*p)) p++;
    *p='\0';
    IsCmd(start,t);
    if (t==0)
    {
      Werror("unknown type `%s`",start);
      omFree(ss);
      omFree(res);
      currRingHdl=save_ring;
      return NULL;
    }
    if (RingDependend(t))
      res->size++;    // one additional field for the ring (before the data)
    //Print("found type %s at real-pos %d",start,res->size);
    elem=(newstruct_member)omAlloc0(sizeof(*elem));
    // read name:
    p++;
    while (*p==' ') p++;
    start=p;
    while (isalpha(*p)) p++;
    c=*p;
    *p='\0';
    elem->typ=t;
    elem->pos=res->size;
    if (*start=='\0') /*empty name*/
    {
      WerrorS("empty name for element");
      omFree(elem);
      omFree(ss);
      omFree(res);
      currRingHdl=save_ring;
      return NULL;
    }
    elem->name=omStrDup(start);
    //Print(" name:%s\n",start);
    elem->next=res->member;
    res->member=elem;
    res->size++;

    // next ?
    *p=c;
    while (*p==' ') p++;
    if (*p!=',') break;
    p++;
  }
  Print("new type with %d elements\n",res->size);
  omFree(ss);
  currRingHdl=save_ring;
  return res;
}
newstruct_desc newstructChildFromString(const char *p, const char *s)
{
  // not yet
  return NULL;
}
