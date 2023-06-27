#include <m_pd.h>

static t_class * minimal_class;

typedef struct _minimal
{
    t_object x_obj;
} t_minimal;

void * minimal_new(void)
{
    t_minimal * x = (t_minimal*)pd_new(minimal_class);
    return (void *) x;
}

void minimal_setup(void)
{
    minimal_class = class_new(
        gensym("minimal"),
        (t_newmethod)minimal_new,
        0,
        sizeof(t_minimal),
        CLASS_DEFAULT,
        0);
}