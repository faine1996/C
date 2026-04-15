#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

/*
 * Description: Converts a valid mathematical infix expression into a postfix expression.
 * Assumes single-character operands (0-9) for simplicity.
 * Input:       _infix - The null-terminated string containing the infix expression.
 * _postfix - A pre-allocated buffer to store the resulting postfix string.
 * Return:      1 upon success, 0 upon failure (e.g., null pointers or stack errors).
 */
int InfixToPostfix(const char* _infix, char* _postfix);

/*
 * Description: Evaluates a mathematical postfix expression and calculates the result.
 * Input:       _postfix - The null-terminated string containing the postfix expression.
 * _result - Pointer to an integer where the final calculated result will be stored.
 * Return:      1 upon success, 0 upon failure (e.g., invalid format, division by zero).
 */
int EvaluatePostfix(const char* _postfix, int* _result);

#endif /* EXPRESSION_PARSER_H */