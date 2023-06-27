#include "m_pd.h"

static t_class *counter_class;

typedef struct _counter {
  t_object  x_obj;
  int i_count;              // personal counter
} t_counter;

void counter_bang(t_counter *x)
{
  t_float f=x->i_count;         // previous value of the counter 
  x->i_count++;                 // increment the counter
  outlet_float(x->x_obj.ob_outlet, f); // push the previous value in the outlet
}

void *counter_new(t_floatarg f)
{
  t_counter *x = (t_counter *)pd_new(counter_class);

  x->i_count=f;
  outlet_new(&x->x_obj, &s_float);      // specifies the outlet type for our counter class

  return (void *)x;
}

void counter_setup(void) {
  counter_class = class_new(gensym("counter"),
        (t_newmethod)counter_new,
        0, sizeof(t_counter),
        CLASS_DEFAULT,
        A_DEFFLOAT,             // tells Pd that the object needs one argument of the type t_floatarg
        0);

  class_addbang(counter_class, counter_bang);
}