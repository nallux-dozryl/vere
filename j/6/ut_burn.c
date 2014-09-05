/* j/6/ut_burn.c
**
** This file is in the public domain.
*/
#include "all.h"


/* logic
*/
  static u3_noun
  _burn_in(
           u3_noun van,
           u3_noun sut,
           u3_noun gil)
  {
    u3_noun p_sut, q_sut, pq_sut, qq_sut, rq_sut, prq_sut, qrq_sut;

    if ( u3_no == u3du(sut) ) switch ( sut ) {
      default: return u3_cm_bail(c3__fail);

      case c3__noun: return 0;
      case c3__void: {
        return u3_cm_error("burn-void");
      }
    }
    else switch ( u3h(sut) ) {
      default: return u3_cm_bail(c3__fail);

      case c3__atom: return 0;
      case c3__bull: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
      {
        return _burn_in(van, q_sut, gil);
      }
      case c3__cell: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
      {
        return u3nc
          (_burn_in(van, p_sut, gil),
                  _burn_in(van, q_sut, gil));
      }
      case c3__core: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
                     u3_cx_trel(q_sut, &pq_sut, &qq_sut, &rq_sut);
                     u3_cx_cell(rq_sut, &prq_sut, &qrq_sut);
      {
        return u3nc
          (u3k(prq_sut),
                  _burn_in(van, p_sut, gil));
      }
      case c3__cube: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
      {
        return u3k(p_sut);
      }
      case c3__face: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
      {
        return _burn_in(van, q_sut, gil);
      }
      case c3__fork: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
      {
        return _burn_in(van, p_sut, gil);
      }
      case c3__hold: p_sut = u3t(sut);
      {
        if ( (u3_yes == u3_cqdi_has(gil, sut)) ) {
          u3_noun sux = u3_cqfu_dunq(van, "type", sut);

          u3_ct_push(u3nc(c3__mean, sux));
          return u3_cm_error("burn-loop");
        }
        else {
          u3_noun zoc = u3_cqdi_put(gil, sut);
          u3_noun fop = u3_cqfu_repo(van, sut);
          u3_noun pro = _burn_in(van, fop, zoc);

          u3z(fop);
          u3z(zoc);

          return pro;
        }
      }
    }
  }

  u3_noun
  _cqfu_burn(
                        u3_noun van,
                        u3_noun sut)
  {
    return _burn_in(van, sut, u3_nul);
  }

  u3_noun
  u3_cqfu_burn(
                        u3_noun van,
                        u3_noun sut)
  {
    c3_m    fun_m = c3__burn;
    u3_noun pro   = u3_cz_find(fun_m, sut);

    if ( u3_none != pro ) {
      return pro;
    }
    else {
      pro = _cqfu_burn(van, sut);

      return u3_cz_save(fun_m, sut, pro);
    }
  }

  u3_noun
  u3_cwfu_burn(
                       u3_noun cor)
  {
    u3_noun sut;

    if ( u3_none == (sut = u3_cr_at(u3_cv_sam, cor)) ) {
      return u3_cm_bail(c3__fail);
    } else {
      return u3_cqfu_burn(cor, sut);
    }
  }
