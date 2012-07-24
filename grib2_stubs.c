/*
 * An OCaml wrapper for NCEP's NCO library
 * Hezekiah M. Carty
 *
 */

/* The "usual" OCaml includes */
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/fail.h>
#include <caml/memory.h>
#include <caml/misc.h>
#include <caml/mlvalues.h>
#include <caml/bigarray.h>
#include <caml/custom.h>

/* Shapelib API include */
#include <grib2.h>

/* For debugging - we want to have access to printf, stderr and such */
#include <stdio.h>
#include <string.h>

// Allow for ridiculously long exception strings.
#define MAX_EXCEPTION_MESSAGE_LENGTH 10000

//
//
// GRIB field handling
//
//

#define Gribfield_val(val) (* ((gribfield **) Data_custom_val(val)))

// OCaml handler to free a GRIB field when it is GC'd
void finalize_gribfield( value ml_field ) {
    gribfield *field;
    field = Gribfield_val( ml_field );
    g2_free( field );
    return;
}

// Definition for custom OCaml handler functions
static struct custom_operations gribfield_custom_ops = {
    identifier: "grib field handling",
    finalize: finalize_gribfield,
    compare: custom_compare_default,
    hash: custom_hash_default,
    serialize: custom_serialize_default,
    deserialize: custom_deserialize_default
};

value Val_gribfield( gribfield *field ) {
    gribfield **store;
    value ret;
    ret = caml_alloc_custom(&gribfield_custom_ops, sizeof(store), 0, 1);
    store = Data_custom_val(ret);
    *store = field;
    return ret;
}

//
//
// Unpacking
//
//

// Get GRIB field
value ml_g2_getfld( value message, value field_num, value unpack, value expand ) {
    CAMLparam4( message, field_num, unpack, expand );

    int result;
    gribfield *field;

    result = g2_getfld( String_val( message ), Int_val( field_num ), Int_val( unpack ), Int_val( expand ), &field );

    // Raise an exception if we fail to extract a field
    if ( result != 0 ) {
        caml_invalid_argument( "Bad message" );
    }

    CAMLreturn( Val_gribfield( field ) );
}

// Get grid values from a GRIB field
value ml_get_data( value ml_field ) {
    CAMLparam1( ml_field );
    CAMLlocal1( ml_data );

    int i;
    gribfield *field;
    field = Gribfield_val( ml_field );

    // Allocate an OCaml array and copy the data over
    ml_data = caml_alloc( field->ndpts * Double_wosize, Double_array_tag );
    for ( i = 0; i < field->ndpts; i++ ) {
        Store_double_field( ml_data, i, field->fld[i] );
    }

    // Return the OCaml-formatted data copy
    CAMLreturn( ml_data );
}

