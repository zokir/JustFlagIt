%module flagit
%{
#include "flagit.hh"
%}

%include "std_string.i"
%include "std_vector.i"
namespace std {
    %template(StringVector) vector<string>;
};

// This is only for python. We might need to split module in base/derived.
%rename("%(utitle)s", %$isfunction, %$ismember) "";

%include exception.i

%exception {
  try {
      $action
  } catch(std::invalid_argument) {
      SWIG_exception(SWIG_AttributeError, "Invalid Argument Error");
  } catch(std::logic_error) {
      SWIG_exception(SWIG_ValueError, "Logic error");
  } catch(...) {
      SWIG_exception(SWIG_RuntimeError, "Unknown exception");
  }
}

%include "src/flagit.hh"