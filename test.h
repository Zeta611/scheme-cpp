#ifndef TEST_H
#define TEST_H

void test();

void test_hash_table();
void test_hash_table_can_insert();
void test_hash_table_can_get();
void test_hash_table_can_count();
void test_hash_table_can_be_full();
void test_hash_table_can_overflow();
void test_hash_table_can_update();
void test_hash_table_can_be_accessed_with_hash_value();

void test_node_pool();
void test_node_pool_can_be_empty();
void test_node_pool_can_get();
void test_node_pool_can_allocate();
void test_node_pool_can_deallocate();
void test_node_pool_can_reserve_capacity();

void test_tokenizer();
void test_tokenizer_can_tokenize_parenthesis();
void test_tokenizer_can_tokenize_whitespaces();
void test_tokenizer_can_tokenize_variables();
void test_tokenizer_can_tokenize_numbers();
void test_tokenizer_can_tokenize_mixed_tokens();
void test_tokenizer_can_put_back_tokens();

#endif /* ifndef TEST_H */
