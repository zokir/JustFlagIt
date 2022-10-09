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
  } catch(std::invalid_argument const& e) {
      SWIG_exception(SWIG_AttributeError, e.what());
  } catch(std::logic_error const& e) {
      SWIG_exception(SWIG_ValueError, e.what());
  } catch(...) {
      SWIG_exception(SWIG_RuntimeError, "Unknown exception");
  }
}

%include "src/flagit.hh"
