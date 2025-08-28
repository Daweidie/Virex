#ifndef TYPE_INFERENCE_H
#define TYPE_INFERENCE_H
#include "ast.h"
typedef enum {
    TYPE_UNKNOWN,
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_STRING
} InferredType;
typedef struct {
    char* name;
    InferredType type;
} VariableInfo;
typedef struct {
    VariableInfo* variables;
    int var_count;
    int var_capacity;
} TypeInferenceContext;
TypeInferenceContext* create_type_inference_context();
void free_type_inference_context(TypeInferenceContext* ctx);
InferredType infer_type(TypeInferenceContext* ctx, ASTNode* node);
InferredType get_variable_type(TypeInferenceContext* ctx, const char* var_name);
void set_variable_type(TypeInferenceContext* ctx, const char* var_name, InferredType type);
const char* type_to_cpp_string(InferredType type);
InferredType infer_type_from_value(const char* value);

#endif