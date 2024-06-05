#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define PY_ARRAY_UNIQUE_SYMBOL Py_Array_API_myext
#include <arrayobject.h>

#include "FDR.h"

int assert_uint_2darray(const PyArrayObject *X, const char title[])
{
	char error_message[1000];
	int flag = 1;

	strcpy(error_message, title);

	if(sizeof(UINT16) != 2)
	{
		strcpy(error_message, "The size of unsigned short 16 must be two bytes");
		flag = 0;

	}else{
		if(X->nd != 2 || X->descr->type_num != PyArray_UINT16)
		{
			strcat(error_message, " must be two-dimensional unsigned integer in 16 bits");
			flag = 0;
		}

		if(X->strides[1] != sizeof(UINT16) || X->strides[0] != X->dimensions[1] * sizeof(UINT16))
		{
			strcat(error_message, " array stride is wrong in the memory");
			flag = 0;
		}
	}

	if(!flag) PyErr_SetString(PyExc_ValueError, error_message);
	return flag;
}


static PyObject *convert(PyObject *self, PyObject *args)
{
	PyArrayObject *result = NULL, *input;
	double *FDR;

	if (!PyArg_ParseTuple(args, "O", &input)) return NULL;
	if (!assert_uint_2darray(input, "input")) return NULL;

	FDR = wrapper_convert_rank_to_FDR((UINT16*)input->data, input->dimensions[0], input->dimensions[1]);

	result = (PyArrayObject*)PyArray_SimpleNewFromData(2, input->dimensions, NPY_DOUBLE, (void*)FDR);

	return PyArray_Return(result);
}


static PyMethodDef FDR_methods[] = {
	{"convert", convert, METH_VARARGS, "Compute FDR for genes based on the consistency between two lists."},
	{NULL, NULL, 0, NULL}  // Sentinel
};

static struct PyModuleDef FDR_module = {
	PyModuleDef_HEAD_INIT,
	"FDR_consistency",   // name of module
	NULL, // module documentation
	-1,       // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
	FDR_methods
};

PyMODINIT_FUNC PyInit_FDR_consistency(void)
{
	import_array();
    return PyModule_Create(&FDR_module);
}


void standalone_run()
{
	// for debugging evaluation of parameter input
	FILE *fp = fopen("/Users/jiangp4/Desktop/out", "r+");
	const UINT16 nrow=2, ncol=2213;

	UINT16 i, j, rank[nrow * ncol];

	double *FDR;

	for(i=0; i<nrow; i++){
		for(j=0; j<ncol; j++) fscanf(fp, "%hu", &rank[i*ncol + j]);
	}
	fclose(fp);

	FDR = wrapper_convert_rank_to_FDR(rank, nrow, ncol);

	fp = fopen("/Users/jiangp4/Desktop/out.FDR", "w+");
	for(i=0; i<nrow; i++){
		for(j=0; j<ncol; j++) fprintf(fp, "%f%c", FDR[i*ncol+j], (j==ncol-1)?'\n':'\t');
	}
	fclose(fp);

	free(FDR);
}


int main(int argc, char *argv[])
{
	wchar_t *program = Py_DecodeLocale(argv[0], NULL);

	if (program == NULL) {
		fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
		exit(1);
	}

	// Add buildin module
	PyImport_AppendInittab("FDR_consistency", PyInit_FDR_consistency);

	// Pass argv[0] to the Python interpreter
	Py_SetProgramName(program);

	Py_Initialize();

	// Optionally import the module; alternatively, import can be deferred until the embedded script imports it
	PyImport_ImportModule("FDR_consistency");

	PyMem_RawFree(program);

	// Debug area
	//standalone_run();

	return 0;
}
