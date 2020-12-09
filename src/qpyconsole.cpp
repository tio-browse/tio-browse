/*
  QPyConsole.cpp

  Controls the GEMBIRD Silver Shield PM USB outlet device

  (C) 2006, Mondrian Nuessle, Computer Architecture Group, University of Mannheim, Germany

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


  nuessle@uni-mannheim.de

*/

// modified by YoungTaek Oh.
// modified by Thomas Cann. 

#define PY_SSIZE_T_CLEAN
#define NPY_NO_DEPRECATED_API  NPY_1_9_API_VERSION 

#ifdef WIN32
#   undef _DEBUG
#endif
#include <Python.h>
#include <numpy/arrayobject.h>
#include "qpyconsole.h"
#include "structmember.h"

#include <QDebug>

PyObject* glb;
PyObject* loc;

QString resultString;

static PyObject* redirector_init(PyObject *, PyObject *)
{
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* redirector_write(PyObject *, PyObject *args)
{
    char* output;
    PyObject *selfi;
 
    if (!PyArg_ParseTuple(args,"s",&output))
    {
        return NULL;
    }

    QString outputString = QString::fromLocal8Bit(output);
    resultString.append(outputString);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef redirectorMethods[] =
{
    {"__init__", redirector_init, METH_VARARGS,
     "initialize the stdout/err redirector"},
    {"write", redirector_write, METH_VARARGS,
     "implement the write method to redirect stdout/err"},
    {NULL,NULL,0,NULL},
};

typedef struct{
    PyObject_HEAD
} RedirectorObject;

static void Redirector_dealloc (RedirectorObject * self){
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject* Redirector_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
    RedirectorObject *self;
    self = (RedirectorObject *) type->tp_alloc(type, 0);
    return (PyObject *)self;
}

static int Redirector_init(RedirectorObject *self, PyObject *args, PyObject *kwds)
{
    return 0;
}

static PyTypeObject RedirectorObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "redirector.redirector",
    .tp_basicsize = sizeof(RedirectorObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) Redirector_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT| Py_TPFLAGS_BASETYPE,
    .tp_doc = "redirector objects",
    .tp_methods = redirectorMethods,
    .tp_init = (initproc) Redirector_init,
    .tp_new = Redirector_new,
    
};

static PyMethodDef ModuleMethods[] = { 
    {NULL,NULL,0,NULL} 
};

static struct PyModuleDef redirector_moduledef = {
    PyModuleDef_HEAD_INIT,
    "redirector",
    "Module to Redirect Stdout", //description
    -1,
    ModuleMethods //
};

PyMODINIT_FUNC PyInit_redirector(void){
    PyObject * m;
    if(PyType_Ready(&RedirectorObject_Type)< 0){
        return NULL;
    }
    m = PyModule_Create(&redirector_moduledef);
    if(m==NULL){
        return NULL;
    }
    if (PyModule_AddObject(m, "redirector", (PyObject *) &RedirectorObject_Type) < 0) {
        Py_DECREF(&RedirectorObject_Type);
        Py_DECREF(m);
        return NULL;
    }
    return m;
}

static PyObject* py_clear(PyObject *, PyObject *)
{
    QPyConsole::getInstance()->clear();
    return Py_None;
}

static PyObject* py_reset(PyObject *, PyObject *)
{
    QPyConsole::getInstance()->reset();
    return Py_None;
}

static PyObject* py_save(PyObject *, PyObject *args)
{
    char* filename;
    if (!PyArg_ParseTuple(args,"s",&filename))
    {
        return NULL;
    }
    QPyConsole::getInstance()->saveScript(filename);
    return Py_None;
}

static PyObject* py_load(PyObject *, PyObject *args)
{
    char* filename;
    if (!PyArg_ParseTuple(args,"s",&filename))
    {
        return NULL;
    }
    QPyConsole::getInstance()->loadScript(filename);

    return Py_None;
}

static PyObject* py_history(PyObject *, PyObject *)
{
    QPyConsole::getInstance()->printHistory();
    return Py_None;
}

static PyObject* py_quit(PyObject *, PyObject *)
{
    resultString="Use reset() to restart the interpreter; otherwise exit your application\n";
    return Py_None;
}

static PyObject* py_version(PyObject *, PyObject *)
{
    PyRun_SimpleString("print(\"Python \" + sys.version + \" on \" + sys.platform)\n");
    return Py_None;
}


static PyMethodDef console_methods[] =  {
    {"clear",py_clear, METH_VARARGS,"clears the console"},
    {"reset",py_reset, METH_VARARGS,"reset the interpreter and clear the console"},
    {"save",py_save, METH_VARARGS,"save commands up to now in given file"},
    {"load",py_load, METH_VARARGS,"load commands from given file"},
    {"history",py_history, METH_VARARGS,"shows the history"},
    {"quit",py_quit, METH_VARARGS,"print information about quitting"},
    {"version",py_version, METH_VARARGS,"print information about versions of python, system and compilers"},
    {NULL, NULL,0,NULL}
};

static struct PyModuleDef console_moduledef = {
    PyModuleDef_HEAD_INIT,
    "console",
    "Methods to Operate the Console", //description
    -1,
    console_methods
};

PyMODINIT_FUNC PyInit_console(void){
    PyObject * m = PyModule_Create(&console_moduledef);
    return m;
}

void QPyConsole::printHistory()
{
    uint index = 1;
    for ( QStringList::Iterator it = history.begin(); it != history.end(); ++it )
    {
        resultString.append(QString("%1\t%2\n").arg(index).arg(*it));
        index ++;
    }
}

QPyConsole *QPyConsole::theInstance = NULL;

QPyConsole *QPyConsole::getInstance(QWidget *parent, const QString& welcomeText)
{
    if (!theInstance)
    {
        theInstance = new QPyConsole(parent, welcomeText);
    }
    return theInstance;
}

//QT Python console constructor (init the QTextEdit & the attributes)
QPyConsole::QPyConsole(QWidget *parent, const QString& welcomeText) :
        QConsole(parent, welcomeText),lines(0)
{
    program = Py_DecodeLocale("Tio-Browse", NULL);
    Py_SetProgramName(program);

    //set the Python Prompt
    setNormalPrompt(true);
    PyImport_AppendInittab("redirector", PyInit_redirector);
    PyImport_AppendInittab("console", PyInit_console);
    Py_Initialize();
    if(PyArray_API == NULL)
    {
        _import_array(); //https://stackoverflow.com/questions/32899621/numpy-capi-error-with-import-array-when-compiling-multiple-modules
    }
    /* NOTE: In previous implementaion, local name and global name
             were allocated separately.  And it causes a problem that
             a function declared in this console cannot be called.  By
             unifying global and local name with __main__.__dict__, we
             can get more natural python console.
    */
    PyObject *module = PyImport_ImportModule("__main__");
    loc = glb = PyModule_GetDict(module);
    
    PyImport_ImportModule("redirector");
    PyImport_ImportModule("console");

    PyImport_ImportModule("rlcompleter");
    PyRun_SimpleString("import sys\n"
                       "import redirector\n"
                       "from console import *\n"
                       "import rlcompleter\n"
                       "completer = rlcompleter.Completer()\n"
                       "sys.path.insert(0, \".\")\n" // add current
                                                     // path
                       "sys.stdout = redirector.redirector()\n"
                       "sys.stderr = sys.stdout\n"
                    //    "print(\"Python \" + sys.version + \" on \" + sys.platform)\n"
                       
        );
}
char save_error_type[1024], save_error_info[1024];

bool
QPyConsole::py_check_for_unexpected_eof()
{
    PyObject *errobj, *errdata, *errtraceback, *pystring;

    /* get latest python exception info */
    PyErr_Fetch(&errobj, &errdata, &errtraceback);

    pystring = NULL;
    if (errobj != NULL &&
        (pystring = PyObject_Str(errobj)) != NULL &&     /* str(object) */  // what is difference between str and repr in python?
         (PyUnicode_Check(pystring))
        )
    {
        PyObject* pStrObj = PyUnicode_AsUTF8String(pystring);
        const char* str = PyBytes_AsString(pStrObj);
        strcpy(save_error_type, str);
        Py_DECREF(pStrObj);
    }
    else
        strcpy(save_error_type, "<unknown exception type>");
    Py_XDECREF(pystring);

    pystring = NULL;
    if (errdata != NULL &&
        (pystring = PyObject_Str(errdata)) != NULL &&
        (PyUnicode_Check(pystring))
        )
        {
        PyObject* pStrObj = PyUnicode_AsUTF8String(pystring);
        const char* str = PyBytes_AsString(pStrObj);
        strcpy(save_error_info, str);
        Py_DECREF(pStrObj);
        }
    else
        strcpy(save_error_info, "<unknown exception data>");
    Py_XDECREF(pystring);

    if (strstr(save_error_type, "exceptions.SyntaxError")!=NULL &&
        strncmp(save_error_info,"('unexpected EOF while parsing',",32)==0)
    {
        return true;
    }
    PyErr_Print ();
    resultString="Error: ";
    resultString.append(save_error_info);
    Py_XDECREF(errobj);
    Py_XDECREF(errdata);         /* caller owns all 3 */
    Py_XDECREF(errtraceback);    /* already NULL'd out */
    return false;
}

//Desctructor
QPyConsole::~QPyConsole()
{
    if (Py_FinalizeEx() < 0)
    {
            exit(120);
    }
    Py_Finalize();
    PyMem_RawFree(program);
    
    // Py_Finalize();
}

//Call the Python interpreter to execute the command
//retrieve back results using the python internal stdout/err redirectory (see above)
QString QPyConsole::interpretCommand(const QString &command, int *res)
{
    PyObject* py_result;
    PyObject* dum;
    bool multiline=false;
    *res = 0;
    if (!command.startsWith('#') && (!command.isEmpty() || (command.isEmpty() && lines!=0) ))
    {

        if(command.contains("license()", Qt::CaseInsensitive)){
            QString result = "WARNING: Can't type LICENSE() as Interactive Prompt is Disabled. \nPlease find full license online.";
            return result;
        }
        if(command.contains("help()", Qt::CaseInsensitive)){
            QString result = 
            "Console Methods include:\n"
            "clear() - clears the console\n"
            "reset() - reset the interpreter and clear the console\n"
            "save() - save commands up to now in given file\n"
            "load() - load commands from given file\n" 
            "history() - shows the history\n" 
            "quit() - print information about quitting\n" 
            "version() - print information about versions of python, system and compilers\n"             
            ;
            return result;
        }
        this->command.append(command);
        py_result=Py_CompileString(this->command.toLocal8Bit().data(),"<stdin>",Py_single_input);
        if (py_result==0)
        {
            multiline=py_check_for_unexpected_eof();
            if (!multiline) {
                if (command.endsWith(':'))
                    multiline = true;
            }

            if (multiline)
            {
                setMultilinePrompt(false);
                this->command.append("\n");
                lines++;
                resultString="";
                QConsole::interpretCommand(command, res);
                return "";
            }
            else
            {
                setNormalPrompt(false);
                *res=-1;
                QString result=resultString;
                resultString="";
                QConsole::interpretCommand(command, res);
                this->command="";
                this->lines=0;
                return result;
            }
        }
        if ( (lines!=0 && command=="") || (this->command!="" && lines==0))
        {
            setNormalPrompt(false);
            this->command="";
            this->lines=0;

            dum = PyEval_EvalCode(py_result, glb, loc);
            Py_XDECREF (dum);
            Py_XDECREF (py_result);
            if (PyErr_Occurred ())
            {
                *res=-1;
                PyErr_Print ();
            }
            QString result=resultString;
            resultString="";
            if (command!="")
                QConsole::interpretCommand(command, res);
            return result;
        }
        else if (lines!=0 && command!="") //following multiliner line
        {
            this->command.append("\n");
            *res=0;
            QConsole::interpretCommand(command, res);
            return "";
        }
        else
        {
            return "";
        }

    }
    else
        return "";
}

QStringList QPyConsole::suggestCommand(const QString &cmd, QString& prefix)
{
    char run[255];
    int n =0;
    QStringList list;
    prefix = "";
    resultString="";
    if (!cmd.isEmpty()) {
        do {
            snprintf(run, 255, "print(completer.complete(\"%s\" , %d ))\n", cmd.toLatin1().data(),n);
            PyRun_SimpleString(run);
            resultString=resultString.trimmed(); //strip trialing newline //resultString does not work until redirector/catcher does.
            if (resultString!="None"){
                list.append(resultString);
                resultString="";
            }else 
            {
                resultString="";
                break;
            }
            n++;
        } while (true);
    }
    list.removeDuplicates();
    return list;
}

void QPyConsole::dataIntoConsole(QString name, const int ND, QList<int> DIMS, int TypeInt, void* data){
    

    // //Inputs
    // const int ND = 2;
    // const int SIZE = 10;
    // npy_intp dims[2]{SIZE, SIZE};
    // int typeint = NPY_DOUBLE; // https://numpy.org/doc/stable/reference/c-api/dtype.html#c.NPY_FLOAT
    // void* data;

    // Debuging ------------------
    // qInfo() << name << ND << DIMS << TypeInt << data ;    
    // int i;
    // for(i=0;i<ND; i++){
        // int x = ((int *)DIMS)[1];
        // qInfo() << x;
    // }
    // QString name;
    // qInfo() << (npy_intp*)dims; 
    // qInfo() << data;
    // npy_intp* dims
    // ---------------------------

    PyObject *m= PyImport_AddModule("__main__");
    npy_intp dims[DIMS.size()];
    for(int i=0;i<DIMS.size(); i++){
        dims[i]=DIMS[i];
    }
    PyObject *pArray = PyArray_SimpleNewFromData(ND, (npy_intp*)dims, TypeInt, data);
    PyArrayObject *np_arr_tocopy = reinterpret_cast<PyArrayObject*>(pArray);
    PyObject* py_copy= PyArray_Copy(np_arr_tocopy);
    PyObject_SetAttrString(m, name.toStdString().c_str(), py_copy);

}
