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

%include "src/flagit.hh"