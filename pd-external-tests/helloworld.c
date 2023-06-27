#include "m_pd.h"

static t_class* helloworld_class;

typedef struct _helloworld {
    t_object  x_obj; // mandatory member variable - used to store internal object properties like the graphical presentation of the object or data about inlets and outlets.
} t_helloworld;

// mandatory manipulator to handle bang/message sent to our class
void helloworld_bang(t_helloworld* x)
{
    (void)x;
    post("Hello world !!");
}

// constructor for our class
void *helloworld_new(void)
{
  t_helloworld *x = (t_helloworld *)pd_new(helloworld_class);

  return (void *)x;
}

// function called once to load our library
void helloworld_setup(void)
{
    helloworld_class = class_new(gensym("helloworld"),
        (t_newmethod)helloworld_new, // constructor of our class
        0,                           // destructor of our class (here none is needed as no memory is dynamically allocated)
        sizeof(t_helloworld),        // indicates the necessary space to store an instance of our class
        CLASS_DEFAULT,               // influences graphical representation of our element
        0);                          // argument to our object (here none)

    class_addbang(helloworld_class, helloworld_bang);
}