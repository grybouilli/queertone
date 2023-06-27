#include <m_pd.h>

static t_class* complexcounter_class;

typedef struct _complexcounter
{
    // interactive members
    t_object    x_obj;

    // personal space
    int         i_count;
    t_float     step;
    int         i_down,
        i_up;

    // outlets
    t_outlet* f_out,
        * b_out;
} t_complexcounter;

void* complexcounter_new(  t_symbol* s,    // the symbolic name used for object creation
                            int argc,       // the number of arguments passed to the object
                            t_atom* argv)   // a pointer to a list of argc atoms
{
    (void)s;
    t_complexcounter * x = (t_complexcounter *)pd_new(complexcounter_class);
    t_float f1 = 0, f2 = 0;

    x->step = 1;

    // defining behavior on instanciation upon umber of arguments passed to class
    switch (argc) {
        default:
        case 3:
            x->step = atom_getfloat(argv + 2);
        case 2:
            f2 = atom_getfloat(argv + 1);
        case 1:
            f1 = atom_getfloat(argv);
            break;
        case 0:
            break;
    }
    if (argc < 2)f2 = f1;
    x->i_down = (f1 < f2) ? f1 : f2;
    x->i_up = (f1 > f2) ? f1 : f2;

    x->i_count = x->i_down;

    // ------------------------------------------------------------- //
    //                      Inlets & Outlets                         //
    
    // The order of the generation of inlets and outlets is important, since it corresponds to the order of inlets and outlets in the graphical representation of the object.
    // defining inlets
    inlet_new(              // creates a new “active” inlet - that is, a class method is called each time a message is sent to it.
        &x->x_obj,          // internal of the inlet's object
        &x->x_obj.ob_pd,    // GUI of the object
        gensym("list"),     // main right inlet selector
        gensym("bound"));   // substitue selector - message to list can be treated as message to bound on the left inlet. Bound must be declared by class_addmethod

    floatinlet_new(    // “passive” inlet for numerical values - that is, parts of the data space memory may be written directly from outside.
        &x->x_obj,      
        &x->step       // address in the data space memory, where other objects can write too.
        );

    // defining outlets
    x->f_out = outlet_new(    // saving pointers returned by outlet_new in the class data space to be used later by the outlet functions.
        &x->x_obj, &s_float);
    x->b_out = outlet_new(&x->x_obj, &s_bang);

    return (void*)x;
}


void complexcounter_reset(t_complexcounter* x)
{
    x->i_count = x->i_down;
}

void complexcounter_set(t_complexcounter* x, t_floatarg f)
{
    x->i_count = f;
}

void complexcounter_bound(t_complexcounter* x, t_floatarg f1, t_floatarg f2)
{
    x->i_down = (f1 < f2) ? f1 : f2;
    x->i_up = (f1 > f2) ? f1 : f2;
}


void counter_bang(t_complexcounter* x)
{
    t_float f = x->i_count;
    int step = x->step;
    x->i_count += step;

    if (x->i_down - x->i_up) {  // if the difference is not zero (either stricly positive or strictly negative)
        if ((step > 0) && (x->i_count > x->i_up)) { // we over stepped
            x->i_count = x->i_down;
            // complexcounter_reset(x);
            outlet_bang(x->b_out);
        }
        else if (x->i_count < x->i_down) { // we under stepped - over stepped negatively
            x->i_count = x->i_up;
            // complexcounter_set(x, x->i_up);
            outlet_bang(x->b_out);
        }
    }
    outlet_float(x->f_out, f);
}

void complexcounter_setup(void)
{
    // ---------------------------------------------- //
    //      setting up new class complexcounter      //
    complexcounter_class = class_new(
        gensym("complexcounter"),
        (t_newmethod)complexcounter_new,
        0,                                  // weird considering we have pointers
        sizeof(t_complexcounter),
        CLASS_DEFAULT,
        A_GIMME,                            // enables a dynamic number of arguments to be passed at object instantiation
        0);


    // ---------------------------------------------- //
    // setting up functions for class complexcounter
    class_addbang(complexcounter_class, counter_bang);

    class_addmethod(complexcounter_class,
        (t_method)complexcounter_reset,
        gensym("reset"),                  // name of the newly set function
        0                                 // list of argument for the function reset
    );

    class_addmethod(complexcounter_class,
        (t_method)complexcounter_set, gensym("set"),
        A_DEFFLOAT, 0);

    class_addmethod(complexcounter_class,
        (t_method)complexcounter_bound, gensym("bound"),
        A_DEFFLOAT, A_DEFFLOAT, 0);


    // ---------------------------------------------- //
    // setting up help section for class complexcounter
    class_sethelpsymbol(complexcounter_class, gensym("help-counter"));

}