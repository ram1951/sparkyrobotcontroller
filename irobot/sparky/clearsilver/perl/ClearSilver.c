/*
 * This file was generated automatically by xsubpp version 1.9508 from the
 * contents of ClearSilver.xs. Do not edit this file, edit ClearSilver.xs instead.
 *
 *	ANY CHANGES MADE HERE WILL BE LOST!
 *
 */

#line 1 "ClearSilver.xs"
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ClearSilver.h"

/* #define DEBUG_MODE 1
 */

typedef struct {
  HDF*      hdf;
  NEOERR* err; 	
} perlHDF; 

typedef struct {
  CSPARSE* cs;
  NEOERR* err;
} perlCS;	

typedef perlHDF* ClearSilver__HDF;
typedef perlCS* ClearSilver__CS;

static char* g_sort_func_name;

static void debug(char* fmt, ...)
{
#ifdef DEBUG_MODE
  va_list argp;
  va_start(argp, fmt);
  vprintf(fmt, argp);
  va_end(argp);
#endif
}

static NEOERR *output (void *ctx, char *s)
{
  sv_catpv((SV*)ctx, s);

  return STATUS_OK;
}

static int sortFunction(const void* in_a, const void* in_b)
{
  HDF** hdf_a;
  HDF** hdf_b;
  perlHDF a, b;
  SV* sv_a;
  SV* sv_b;	
  int count;	
  int ret;

  dSP;

  hdf_a = (HDF**)in_a;
  hdf_b = (HDF**)in_b;

  /* convert to a type Perl can access */
  a.hdf = *hdf_a;
  a.err = STATUS_OK;
  b.hdf = *hdf_b;
  b.err = STATUS_OK;

  ENTER;
  SAVETMPS;

  PUSHMARK(SP);
  sv_a = sv_newmortal();
  sv_setref_pv(sv_a, "ClearSilver::HDF", (void*)&a);

  sv_b = sv_newmortal();
  sv_setref_pv(sv_b, "ClearSilver::HDF", (void*)&b);

  XPUSHs(sv_a);
  XPUSHs(sv_b);

  PUTBACK;

  count = call_pv(g_sort_func_name, G_SCALAR);

  SPAGAIN;

  if (count != 1)
    croak("Big trouble\n");

  PUTBACK;

  ret = POPi;

  FREETMPS;
  LEAVE;

  return ret;
}





#line 109 "ClearSilver.c"
XS(XS_ClearSilver__HDF_new); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_new)
{
    dXSARGS;
    if (items != 1)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::new(self)");
    {
	char*	self = (char *)SvPV_nolen(ST(0));
#line 105 "ClearSilver.xs"
	ClearSilver__HDF hdf;
#line 120 "ClearSilver.c"
	ClearSilver__HDF	RETVAL;
#line 107 "ClearSilver.xs"
	debug("%s\n", self);
	hdf = (ClearSilver__HDF)malloc(sizeof(perlHDF));
	if (!hdf) {
	  RETVAL = NULL;
	} else {
	  hdf->err = hdf_init(&(hdf->hdf));
	  RETVAL = hdf;
	}
#line 131 "ClearSilver.c"
	ST(0) = sv_newmortal();
	sv_setref_pv(ST(0), "ClearSilver::HDF", (void*)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_DESTROY); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_DESTROY)
{
    dXSARGS;
    if (items != 1)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::DESTROY(hdf)");
    {
	ClearSilver__HDF	hdf;

	if (SvROK(ST(0))) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not a reference");
#line 122 "ClearSilver.xs"
        debug("hdf_DESTROY:%x\n", hdf);
        hdf_destroy(&(hdf->hdf));
#line 156 "ClearSilver.c"
    }
    XSRETURN_EMPTY;
}

XS(XS_ClearSilver__HDF_setValue); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_setValue)
{
    dXSARGS;
    if (items != 3)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::setValue(hdf, key, value)");
    {
	ClearSilver__HDF	hdf;
	char*	key = (char *)SvPV_nolen(ST(1));
	char*	value = (char *)SvPV_nolen(ST(2));
	int	RETVAL;
	dXSTARG;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 132 "ClearSilver.xs"
        hdf->err = hdf_set_value(hdf->hdf, key, value);
	if (hdf->err == STATUS_OK) {
	    RETVAL = 0;
	} else {
	    RETVAL = 1;
	}	
#line 187 "ClearSilver.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_getValue); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_getValue)
{
    dXSARGS;
    if (items != 3)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::getValue(hdf, key, default_value)");
    {
	ClearSilver__HDF	hdf;
	char*	key = (char *)SvPV_nolen(ST(1));
	char*	default_value = (char *)SvPV_nolen(ST(2));
	char *	RETVAL;
	dXSTARG;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 148 "ClearSilver.xs"
        RETVAL = hdf_get_value(hdf->hdf, key, default_value);
#line 214 "ClearSilver.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_copy); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_copy)
{
    dXSARGS;
    if (items != 3)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::copy(hdf, name, src)");
    {
	ClearSilver__HDF	hdf;
	char*	name = (char *)SvPV_nolen(ST(1));
	ClearSilver__HDF	src;
	int	RETVAL;
	dXSTARG;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");

	if (sv_derived_from(ST(2), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(2)));
	    src = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "src is not of type ClearSilver::HDF");
#line 159 "ClearSilver.xs"
        hdf->err = hdf_copy(hdf->hdf, name, src->hdf);
        if (hdf->err == STATUS_OK) {
            RETVAL = 0;
        } else {
            RETVAL = 1;
        }
#line 253 "ClearSilver.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_readFile); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_readFile)
{
    dXSARGS;
    if (items != 2)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::readFile(hdf, filename)");
    {
	ClearSilver__HDF	hdf;
	char*	filename = (char *)SvPV_nolen(ST(1));
	int	RETVAL;
	dXSTARG;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 173 "ClearSilver.xs"
        hdf->err = hdf_read_file(hdf->hdf, filename);
	if (hdf->err == STATUS_OK) {
	    RETVAL = 1;
	} else {
	    RETVAL = 0;
	}	
#line 284 "ClearSilver.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_writeFile); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_writeFile)
{
    dXSARGS;
    if (items != 2)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::writeFile(hdf, filename)");
    {
	ClearSilver__HDF	hdf;
	char*	filename = (char *)SvPV_nolen(ST(1));
	int	RETVAL;
	dXSTARG;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 187 "ClearSilver.xs"
        hdf->err = hdf_write_file(hdf->hdf, filename);
       if (hdf->err == STATUS_OK) {
           RETVAL = 1;
       } else {
           RETVAL = 0;
       }
#line 315 "ClearSilver.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_getObj); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_getObj)
{
    dXSARGS;
    if (items != 2)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::getObj(hdf, name)");
    {
	ClearSilver__HDF	hdf;
	char*	name = (char *)SvPV_nolen(ST(1));
#line 201 "ClearSilver.xs"
	HDF* tmp_hdf;
	perlHDF* perlhdf;
#line 333 "ClearSilver.c"
	ClearSilver__HDF	RETVAL;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 204 "ClearSilver.xs"
	do {
	    tmp_hdf = hdf_get_obj(hdf->hdf, name);
	    if (!tmp_hdf) {
	        RETVAL = NULL;
		break;
	    }
	    perlhdf = (perlHDF*)malloc(sizeof(perlHDF));
	    if (!perlhdf) {
	        RETVAL = NULL;
	        break;
	    }
            perlhdf->hdf = tmp_hdf;
	    perlhdf->err = STATUS_OK;
	    RETVAL = perlhdf;
	} while (0);
#line 358 "ClearSilver.c"
	ST(0) = sv_newmortal();
	sv_setref_pv(ST(0), "ClearSilver::HDF", (void*)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_objChild); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_objChild)
{
    dXSARGS;
    if (items != 1)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::objChild(hdf)");
    {
	ClearSilver__HDF	hdf;
#line 226 "ClearSilver.xs"
	HDF* tmp_hdf;
	perlHDF* child;
#line 376 "ClearSilver.c"
	ClearSilver__HDF	RETVAL;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 229 "ClearSilver.xs"
	do {
	    tmp_hdf = hdf_obj_child(hdf->hdf);
	    if (!tmp_hdf) {
	        RETVAL = NULL;
		break;
	    }
	    child = (perlHDF*)malloc(sizeof(perlHDF));
	    if (!child) {
	        RETVAL = NULL;
	        break;
	    }
            child->hdf = tmp_hdf;
	    child->err = STATUS_OK;
	    RETVAL = child;
	} while (0);
#line 401 "ClearSilver.c"
	ST(0) = sv_newmortal();
	sv_setref_pv(ST(0), "ClearSilver::HDF", (void*)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_getChild); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_getChild)
{
    dXSARGS;
    if (items != 2)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::getChild(hdf, name)");
    {
	ClearSilver__HDF	hdf;
	char*	name = (char *)SvPV_nolen(ST(1));
#line 253 "ClearSilver.xs"
	HDF* tmp_hdf;
	perlHDF* child;
#line 420 "ClearSilver.c"
	ClearSilver__HDF	RETVAL;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 256 "ClearSilver.xs"
	do {
	    tmp_hdf = hdf_get_child(hdf->hdf, name);
	    if (!tmp_hdf) {
	        RETVAL = NULL;
		break;
	    }
	    child = (perlHDF*)malloc(sizeof(perlHDF));
	    if (!child) {
	        RETVAL = NULL;
	        break;
	    }
            child->hdf = tmp_hdf;
	    child->err = STATUS_OK;
	    RETVAL = child;
	} while (0);
#line 445 "ClearSilver.c"
	ST(0) = sv_newmortal();
	sv_setref_pv(ST(0), "ClearSilver::HDF", (void*)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_objValue); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_objValue)
{
    dXSARGS;
    if (items != 1)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::objValue(hdf)");
    {
	ClearSilver__HDF	hdf;
	char *	RETVAL;
	dXSTARG;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 278 "ClearSilver.xs"
	RETVAL = hdf_obj_value(hdf->hdf);
#line 471 "ClearSilver.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_objName); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_objName)
{
    dXSARGS;
    if (items != 1)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::objName(hdf)");
    {
	ClearSilver__HDF	hdf;
	char *	RETVAL;
	dXSTARG;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 286 "ClearSilver.xs"
	RETVAL = hdf_obj_name(hdf->hdf);
#line 496 "ClearSilver.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_objNext); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_objNext)
{
    dXSARGS;
    if (items != 1)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::objNext(hdf)");
    {
	ClearSilver__HDF	hdf;
#line 294 "ClearSilver.xs"
	HDF* tmp_hdf;
	perlHDF* next;
#line 513 "ClearSilver.c"
	ClearSilver__HDF	RETVAL;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 297 "ClearSilver.xs"
	do {
	    tmp_hdf = hdf_obj_next(hdf->hdf);
	    if (!tmp_hdf) {
	        RETVAL = NULL;
		break;
	    }
	    next = (perlHDF*)malloc(sizeof(perlHDF));
	    if (!next) {
	      RETVAL = NULL;
	      break;
            }
	    next->hdf = tmp_hdf;
	    next->err = STATUS_OK;
	    RETVAL = next;
	} while (0);
#line 538 "ClearSilver.c"
	ST(0) = sv_newmortal();
	sv_setref_pv(ST(0), "ClearSilver::HDF", (void*)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_sortObj); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_sortObj)
{
    dXSARGS;
    if (items != 2)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::sortObj(hdf, func_name)");
    {
	ClearSilver__HDF	hdf;
	char*	func_name = (char *)SvPV_nolen(ST(1));
#line 320 "ClearSilver.xs"
        NEOERR* err;
#line 556 "ClearSilver.c"
	int	RETVAL;
	dXSTARG;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 322 "ClearSilver.xs"
	g_sort_func_name = func_name;
        err = hdf_sort_obj(hdf->hdf, sortFunction);
        RETVAL = 0;
#line 570 "ClearSilver.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_setSymlink); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_setSymlink)
{
    dXSARGS;
    if (items != 3)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::setSymlink(hdf, src, dest)");
    {
	ClearSilver__HDF	hdf;
	char*	src = (char *)SvPV_nolen(ST(1));
	char*	dest = (char *)SvPV_nolen(ST(2));
#line 335 "ClearSilver.xs"
	NEOERR* err;
#line 588 "ClearSilver.c"
	int	RETVAL;
	dXSTARG;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 337 "ClearSilver.xs"
      	err = hdf_set_symlink (hdf->hdf, src, dest);
       	if (err == STATUS_OK) {
       	    RETVAL = 1;
       	} else {
       	    RETVAL = 0;
       	}
#line 605 "ClearSilver.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__HDF_removeTree); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__HDF_removeTree)
{
    dXSARGS;
    if (items != 2)
	Perl_croak(aTHX_ "Usage: ClearSilver::HDF::removeTree(hdf, name)");
    {
	ClearSilver__HDF	hdf;
	char*	name = (char *)SvPV_nolen(ST(1));
#line 352 "ClearSilver.xs"
        NEOERR* err;
#line 622 "ClearSilver.c"
	int	RETVAL;
	dXSTARG;

	if (sv_derived_from(ST(0), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 354 "ClearSilver.xs"
        err = hdf_remove_tree(hdf->hdf, name);
       	if (err == STATUS_OK) {
       	    RETVAL = 1;
       	} else {
       	    RETVAL = 0;
       	}
#line 639 "ClearSilver.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__CS_new); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__CS_new)
{
    dXSARGS;
    if (items != 2)
	Perl_croak(aTHX_ "Usage: ClearSilver::CS::new(self, hdf)");
    {
	char*	self = (char *)SvPV_nolen(ST(0));
	ClearSilver__HDF	hdf;
#line 371 "ClearSilver.xs"
        perlCS* cs;
#line 656 "ClearSilver.c"
	ClearSilver__CS	RETVAL;

	if (sv_derived_from(ST(1), "ClearSilver::HDF")) {
	    IV tmp = SvIV((SV*)SvRV(ST(1)));
	    hdf = INT2PTR(ClearSilver__HDF,tmp);
	}
	else
	    Perl_croak(aTHX_ "hdf is not of type ClearSilver::HDF");
#line 373 "ClearSilver.xs"
	cs = (perlCS*)malloc(sizeof(perlCS));
	if (!cs) {
	  RETVAL = NULL;
	} else {
	  cs->err = cs_init(&(cs->cs), hdf->hdf);
	  if (cs->err == STATUS_OK) {
	    cs->err = cgi_register_strfuncs(cs->cs);
	  }
	  RETVAL = cs;
	}
#line 676 "ClearSilver.c"
	ST(0) = sv_newmortal();
	sv_setref_pv(ST(0), "ClearSilver::CS", (void*)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__CS_DESTROY); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__CS_DESTROY)
{
    dXSARGS;
    if (items != 1)
	Perl_croak(aTHX_ "Usage: ClearSilver::CS::DESTROY(cs)");
    {
	ClearSilver__CS	cs;

	if (SvROK(ST(0))) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    cs = INT2PTR(ClearSilver__CS,tmp);
	}
	else
	    Perl_croak(aTHX_ "cs is not a reference");
#line 390 "ClearSilver.xs"
	debug("perlcs_DESTROY() is called\n");
	cs_destroy(&(cs->cs));
#line 701 "ClearSilver.c"
    }
    XSRETURN_EMPTY;
}

XS(XS_ClearSilver__CS_displayError); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__CS_displayError)
{
    dXSARGS;
    if (items != 1)
	Perl_croak(aTHX_ "Usage: ClearSilver::CS::displayError(cs)");
    {
	ClearSilver__CS	cs;

	if (sv_derived_from(ST(0), "ClearSilver::CS")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    cs = INT2PTR(ClearSilver__CS,tmp);
	}
	else
	    Perl_croak(aTHX_ "cs is not of type ClearSilver::CS");
#line 397 "ClearSilver.xs"
	nerr_log_error(cs->err);
#line 723 "ClearSilver.c"
    }
    XSRETURN_EMPTY;
}

XS(XS_ClearSilver__CS_render); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__CS_render)
{
    dXSARGS;
    if (items != 1)
	Perl_croak(aTHX_ "Usage: ClearSilver::CS::render(cs)");
    {
	ClearSilver__CS	cs;
	char *	RETVAL;
	dXSTARG;

	if (sv_derived_from(ST(0), "ClearSilver::CS")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    cs = INT2PTR(ClearSilver__CS,tmp);
	}
	else
	    Perl_croak(aTHX_ "cs is not of type ClearSilver::CS");
#line 403 "ClearSilver.xs"
    {
	SV *str = newSV(0);
	cs->err = cs_render(cs->cs, str, output);
	if (cs->err == STATUS_OK) {
	  ST(0) = sv_2mortal(str);
	} else {
	  SvREFCNT_dec(str);
	  ST(0) = &PL_sv_undef;
	}
	XSRETURN(1);
    }
#line 757 "ClearSilver.c"
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__CS_parseFile); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__CS_parseFile)
{
    dXSARGS;
    if (items != 2)
	Perl_croak(aTHX_ "Usage: ClearSilver::CS::parseFile(cs, cs_file)");
    {
	ClearSilver__CS	cs;
	char*	cs_file = (char *)SvPV_nolen(ST(1));
	int	RETVAL;
	dXSTARG;

	if (sv_derived_from(ST(0), "ClearSilver::CS")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    cs = INT2PTR(ClearSilver__CS,tmp);
	}
	else
	    Perl_croak(aTHX_ "cs is not of type ClearSilver::CS");
#line 420 "ClearSilver.xs"
	do {
	    cs->err =  cs_parse_file(cs->cs, cs_file);
	    if (cs->err != STATUS_OK) {
	        cs->err = nerr_pass(cs->err);
		RETVAL = 0;
		break;
	    }
	    RETVAL = 1;
        } while (0);
#line 790 "ClearSilver.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

XS(XS_ClearSilver__CS_parseString); /* prototype to pass -Wmissing-prototypes */
XS(XS_ClearSilver__CS_parseString)
{
    dXSARGS;
    if (items != 2)
	Perl_croak(aTHX_ "Usage: ClearSilver::CS::parseString(cs, in_str)");
    {
	ClearSilver__CS	cs;
	char*	in_str = (char *)SvPV_nolen(ST(1));
#line 437 "ClearSilver.xs"
	char* cs_str;
	int len;
#line 808 "ClearSilver.c"
	int	RETVAL;
	dXSTARG;

	if (sv_derived_from(ST(0), "ClearSilver::CS")) {
	    IV tmp = SvIV((SV*)SvRV(ST(0)));
	    cs = INT2PTR(ClearSilver__CS,tmp);
	}
	else
	    Perl_croak(aTHX_ "cs is not of type ClearSilver::CS");
#line 440 "ClearSilver.xs"
	do {
	    len = strlen(in_str);
	    cs_str = (char *)malloc(len);
	    if (!cs_str) {
	        RETVAL = 0;
		break;
	    }
	    strcpy(cs_str, in_str);
            cs->err =  cs_parse_string(cs->cs, cs_str, len);
	    if (cs->err != STATUS_OK)
		RETVAL = 0;
	    RETVAL = 1;
       } while (0);
#line 832 "ClearSilver.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

#ifdef __cplusplus
extern "C"
#endif
XS(boot_ClearSilver); /* prototype to pass -Wmissing-prototypes */
XS(boot_ClearSilver)
{
    dXSARGS;
    char* file = __FILE__;

    XS_VERSION_BOOTCHECK ;

        newXS("ClearSilver::HDF::new", XS_ClearSilver__HDF_new, file);
        newXS("ClearSilver::HDF::DESTROY", XS_ClearSilver__HDF_DESTROY, file);
        newXS("ClearSilver::HDF::setValue", XS_ClearSilver__HDF_setValue, file);
        newXS("ClearSilver::HDF::getValue", XS_ClearSilver__HDF_getValue, file);
        newXS("ClearSilver::HDF::copy", XS_ClearSilver__HDF_copy, file);
        newXS("ClearSilver::HDF::readFile", XS_ClearSilver__HDF_readFile, file);
        newXS("ClearSilver::HDF::writeFile", XS_ClearSilver__HDF_writeFile, file);
        newXS("ClearSilver::HDF::getObj", XS_ClearSilver__HDF_getObj, file);
        newXS("ClearSilver::HDF::objChild", XS_ClearSilver__HDF_objChild, file);
        newXS("ClearSilver::HDF::getChild", XS_ClearSilver__HDF_getChild, file);
        newXS("ClearSilver::HDF::objValue", XS_ClearSilver__HDF_objValue, file);
        newXS("ClearSilver::HDF::objName", XS_ClearSilver__HDF_objName, file);
        newXS("ClearSilver::HDF::objNext", XS_ClearSilver__HDF_objNext, file);
        newXS("ClearSilver::HDF::sortObj", XS_ClearSilver__HDF_sortObj, file);
        newXS("ClearSilver::HDF::setSymlink", XS_ClearSilver__HDF_setSymlink, file);
        newXS("ClearSilver::HDF::removeTree", XS_ClearSilver__HDF_removeTree, file);
        newXS("ClearSilver::CS::new", XS_ClearSilver__CS_new, file);
        newXS("ClearSilver::CS::DESTROY", XS_ClearSilver__CS_DESTROY, file);
        newXS("ClearSilver::CS::displayError", XS_ClearSilver__CS_displayError, file);
        newXS("ClearSilver::CS::render", XS_ClearSilver__CS_render, file);
        newXS("ClearSilver::CS::parseFile", XS_ClearSilver__CS_parseFile, file);
        newXS("ClearSilver::CS::parseString", XS_ClearSilver__CS_parseString, file);
    XSRETURN_YES;
}

