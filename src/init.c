#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* FIXME: 
   Check these declarations against the C/Fortran source code.
*/

/* .Call calls */
extern SEXP call_brute_solver(SEXP);
extern SEXP call_human_solver(SEXP);
extern SEXP call_solver(SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"call_brute_solver", (DL_FUNC) &call_brute_solver, 1},
    {"call_human_solver", (DL_FUNC) &call_human_solver, 1},
    {"call_solver",       (DL_FUNC) &call_solver,       1},
    {NULL, NULL, 0}
};

void R_init_sudsolve(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
