#include <Python.h>
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Parse.h>


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
    static PyObject *RRunnerError;

    static PyObject* _run_R(PyObject *self, PyObject *args) {
        char* code;
        SEXP e, tmp, rRes;
        PyObject* pyRes;
        R_len_t i;
        int hasError;
        ParseStatus status;
        if (!PyArg_ParseTuple(args, "s", &code)) return NULL;
        PROTECT(tmp = mkString(code));
        PROTECT(e = R_ParseVector(tmp, -1, &status, R_NilValue));
        if(status != PARSE_OK) {
            PyErr_SetString(RRunnerError, "Error when parsing R code");
            return NULL;
        } else {
            for(i=0; i<length(e); i++) {
                rRes = R_tryEval(VECTOR_ELT(e,i), R_GlobalEnv, &hasError);
                if(hasError) break;
            }
            if(hasError) {
                PyErr_SetString(RRunnerError, "Error when executing R code");
                return NULL;
            } else if(isString(rRes)) {
                pyRes = Py_BuildValue("s", CHAR(STRING_ELT(rRes,0)));
            } else if(isReal(rRes)) {
                pyRes = PyTuple_New(length(rRes));
                for(Py_ssize_t rl=0; rl<length(rRes); ++rl) {
                    PyTuple_SetItem(pyRes, rl, PyFloat_FromDouble(REAL(rRes)[rl]));
                }
            } else {
                pyRes = Py_BuildValue("s", "Cannot handle return type");
                Py_INCREF(pyRes);
            }
        }
        UNPROTECT(2);
        return pyRes;
    }

    static PyMethodDef EmbeddedRMethods[] = {
        {"run_R", _run_R, METH_VARARGS|METH_KEYWORDS, "Run Some R code."},
        {NULL, NULL, 0, NULL}
    };

    PyMODINIT_FUNC initPythonEmbeddedR(void) {
        PyObject *m;
        m = Py_InitModule("PythonEmbeddedR", EmbeddedRMethods);
        if (m == NULL) return;
        RRunnerError = PyErr_NewException("runr.error", NULL, NULL);
        Py_INCREF(RRunnerError);
        PyModule_AddObject(m, "error", RRunnerError);
    }
#ifdef __cplusplus
} // end extern "C"
#endif // __cplusplus
