#include <flext.h>

// check for appropriate flext version
#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif

class cppcounter:
	// inherit from basic flext class
	public flext_base
{
	// obligatory flext header (class name,base class name)
	FLEXT_HEADER(cppcounter,flext_base)
 
public:
	// constructor
	cppcounter() 
    : _step {1.f}
    , _floor {0.f}
    , _ceil { 10.f}
	{ 
		// define inlets:
		// first inlet must always be of type anything (or signal for dsp objects)
		AddInBang();  // add one inlet for any message
		AddInFloat(3);
		// AddInFloat();
		// AddInFloat();
		// define outlets:
		AddOutFloat();  // add one float outlet (has index 0)
		
		// register methods
        FLEXT_ADDBANG(0, _bang_step);
		FLEXT_ADDMETHOD(1,_set_step);  
		FLEXT_ADDMETHOD(2,_set_floor);  
		FLEXT_ADDMETHOD(3,_set_ceil);  
	} 

protected:
    void _bang_step()
    {
        _counter += _step;
        if(_step > 0 && _counter > _ceil)
        {
            _counter = _floor;
        } else if (_step < 0 && _counter < _floor)
        {
            _counter = _ceil;
        }

        ToOutFloat(0, _counter);
    }
	void _set_step(float step)
    {
        _step = step;
    }

    void _set_floor(float floor)
    {
        _floor = floor;
    }

    void _set_ceil(float ceil)
    {
        _ceil = ceil;
    }

private:
    float   _counter, _step, _floor, _ceil;

    // somehow the callbacks are needed, otherwise it doesn't compile
    FLEXT_CALLBACK(_bang_step)
    FLEXT_CALLBACK_1(_set_step,float)
    FLEXT_CALLBACK_1(_set_floor,float)
    FLEXT_CALLBACK_1(_set_ceil,float)
};

FLEXT_NEW("cppcounter",cppcounter)
