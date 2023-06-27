#include "m_pd.h"

static t_class* ccounter_class;

typedef struct _ccounter {
    t_object  x_obj;
    int i_count;
    t_float step;
    int i_down, i_up;
    t_outlet* f_out, * b_out;
} t_ccounter;

void ccounter_bang(t_ccounter* x)
{
    t_float f = x->i_count;
    int step = x->step;
    x->i_count += step;

    post("stepping ...");
    post(" counter is %d, lower is %d, higher is %d", x->i_count, x->i_down, x->i_up);

    if (x->i_down - x->i_up) {
        if ((step > 0) && (x->i_count > x->i_up)) {
            x->i_count = x->i_down;
            outlet_bang(x->b_out);
        }
        else if (x->i_count < x->i_down) {
            x->i_count = x->i_up;
            outlet_bang(x->b_out);
        }
    }

    outlet_float(x->f_out, f);
}

void ccounter_reset(t_ccounter* x)
{
    x->i_count = x->i_down;
}

void ccounter_set(t_ccounter* x, t_floatarg f)
{
    x->i_count = f;
}

void ccounter_bound(t_ccounter* x, t_floatarg f1, t_floatarg f2)
{
    x->i_down = (f1 < f2) ? f1 : f2;
    x->i_up = (f1 > f2) ? f1 : f2;
}

void* ccounter_new(t_symbol* s, int argc, t_atom* argv)
{
    t_ccounter* x = (t_ccounter*)pd_new(ccounter_class);
    t_float f1 = 0, f2 = 0;

    x->step = 1;
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

    inlet_new(&x->x_obj, &x->x_obj.ob_pd,
        gensym("list"), gensym("bound"));
    floatinlet_new(&x->x_obj, &x->step);

    x->f_out = outlet_new(&x->x_obj, &s_float);
    x->b_out = outlet_new(&x->x_obj, &s_bang);

    return (void*)x;
}

void ccounter_setup(void) {
    ccounter_class = class_new(gensym("ccounter"),
        (t_newmethod)ccounter_new,
        0, sizeof(t_ccounter),
        CLASS_DEFAULT,
        A_GIMME, 0);

    class_addbang(ccounter_class, ccounter_bang);
    class_addmethod(ccounter_class,
        (t_method)ccounter_reset, gensym("reset"), 0);
    class_addmethod(ccounter_class,
        (t_method)ccounter_set, gensym("set"),
        A_DEFFLOAT, 0);
    class_addmethod(ccounter_class,
        (t_method)ccounter_bound, gensym("bound"),
        A_DEFFLOAT, A_DEFFLOAT, 0);

    class_sethelpsymbol(ccounter_class, gensym("help-ccounter"));
}
