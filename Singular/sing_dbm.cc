/****************************************
*  Computer Algebra System SINGULAR     *
****************************************/

//**************************************************************************/
//
// $Id: sing_dbm.cc,v 1.15 2000-12-12 08:44:53 obachman Exp $
//
//**************************************************************************/
//  'sing_dbm.cc' containes command to handle dbm-files under
// Singular. 
//
//**************************************************************************/

#  include <stdio.h>
#  include <fcntl.h>
#  include <errno.h>

#include "mod2.h"

#ifdef HAVE_DBM

#  include "tok.h"
#  include "febase.h"
#  include "omalloc.h"
#  include "ipid.h"
#  include "silink.h"
#  include "sing_dbm.h"

/* These are the routines in dbm. */
#  include "ndbm.h"
typedef struct {
  DBM *db;        // pointer to open database
  int first;      // firstkey to look for?
} DBM_info;

//**************************************************************************/
LINKAGE BOOLEAN dbOpen(si_link l, short flag)
{
  char *mode = "r";
  DBM_info *db;
  int dbm_flags = O_RDONLY | O_CREAT;  // open database readonly as default

  if((l->mode!=NULL)
  && ((l->mode[0]=='w')||(l->mode[1]=='w')))
  {
    dbm_flags = O_RDWR | O_CREAT;
    mode = "rw";
    flag|=SI_LINK_WRITE|SI_LINK_READ;
  }
  else if(flag & SI_LINK_WRITE)
  {
    // request w- open, but mode is not "w" nor "rw" => fail
    return TRUE;
  }
  //if (((db = (DBM_info *)omAlloc(sizeof *db)) != NULL)
  //&&((db->db = dbm_open(l->name, dbm_flags, 0664 )) != NULL ))
  db = (DBM_info *)omAlloc(sizeof *db);
  if((db->db = dbm_open(l->name, dbm_flags, 0664 )) != NULL )
  {
    db->first=1;
    if(flag & SI_LINK_WRITE)
      SI_LINK_SET_RW_OPEN_P(l);
    else
      SI_LINK_SET_R_OPEN_P(l);
    l->data=(void *)(db);
    omFree(l->mode);
    l->mode=omStrDup(mode);
    return FALSE;
  }
  return TRUE;
}

//**************************************************************************/
LINKAGE BOOLEAN dbClose(si_link l)
{
  DBM_info *db = (DBM_info *)l->data;

  dbm_close(db->db);
  omFreeSize((ADDRESS)db,(sizeof *db));
  l->data=NULL;  
  SI_LINK_SET_CLOSE_P(l);
  return FALSE;
}

//**************************************************************************/
static datum d_value;
LINKAGE leftv dbRead2(si_link l, leftv key)
{
  DBM_info *db = (DBM_info *)l->data;
  leftv v=NULL;
  datum d_key;

  if(key!=NULL)
  {
    if (key->Typ()==STRING_CMD)
    {
      d_key.dptr = (char*)key->Data();
      d_key.dsize = strlen(d_key.dptr)+1;
      d_value = dbm_fetch(db->db, d_key);
      v=(leftv)omAlloc0Bin(sleftv_bin);
      if (d_value.dptr!=NULL) v->data=omStrDup(d_value.dptr);
      else                    v->data=omStrDup("");
      v->rtyp=STRING_CMD;
    }
    else
    {
      WerrorS("read(`DBM link`,`string`) expected");
    }
  }
  else
  {
    if(db->first)
      d_value = dbm_firstkey((DBM *)db->db);
    else
      d_value = dbm_nextkey((DBM *)db->db);

    v=(leftv)omAlloc0Bin(sleftv_bin);
    v->rtyp=STRING_CMD;
    if (d_value.dptr!=NULL)
    {
      v->data=omStrDup(d_value.dptr);
      db->first = 0;
    }
    else
    {
      v->data=omStrDup("");
      db->first = 1;
    }

  }
  return v;
}
LINKAGE leftv dbRead1(si_link l)
{
  return dbRead2(l,NULL);
}
//**************************************************************************/
LINKAGE BOOLEAN dbWrite(si_link l, leftv key)
{
  DBM_info *db = (DBM_info *)l->data;
  BOOLEAN b=TRUE;
  register int ret;

  // database is opened
  if((key!=NULL) && (key->Typ()==STRING_CMD) )
  { 
    if (key->next!=NULL)                   // have a second parameter ?
    {
      if(key->next->Typ()==STRING_CMD)     // replace (key,value)
      {
        datum d_key, d_value;

        d_key.dptr = (char *)key->Data();
        d_key.dsize = strlen(d_key.dptr)+1;
        d_value.dptr = (char *)key->next->Data();
        d_value.dsize = strlen(d_value.dptr)+1;
        ret  = dbm_store(db->db, d_key, d_value, DBM_REPLACE);
        if(!ret )
          b=FALSE;
        else
        {
          if(dbm_error(db->db))
          {
            Werror("DBM link I/O error. Is '%s' readonly?", l->name);
            dbm_clearerr(db->db);
          }
        }
      }
    }
    else
    {                               // delete (key)
      datum d_key;

      d_key.dptr = (char *)key->Data();
      d_key.dsize = strlen(d_key.dptr)+1;
      dbm_delete(db->db, d_key);
      b=FALSE;
    }
  }
  else
  {
    WerrorS("write(`DBM link`,`key string` [,`data string`]) expected");
  }
  return b;
}
//**************************************************************************/
//char *dbStatus(si_link l, char *request)
//{
//  if (strcmp(request, "read") == 0)
//  {
//    if (SI_LINK_R_OPEN_P(l))
//      return "ready";
//    else
//      return "not ready";
//  }
//  else if (strcmp(request, "write") == 0)
//  {
//    if (SI_LINK_W_OPEN_P(l))
//      return "ready";
//    else
//      return "not ready";
//  }
//  else return "unknown status request";
//}
//**************************************************************************/

#endif /* HAVE_DBM */
