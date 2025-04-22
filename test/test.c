#include <ctest.h>

#include "converter.h"
#include "dir.h"
#include "list.h"
#include "parser.h"

CTEST(argv_parse, check_no_argv)
{
    char* argv[] = {"./dse"};
    int argc = 1;
    argv_t result = argv_parse(argc, argv);
    ASSERT_NULL(result.opt);
    ASSERT_NULL(result.dir);
}

CTEST(argv_parse, check_only_option)
{
    char* argv[] = {"./dse", "-h"};
    int argc = 2;
    argv_t result = argv_parse(argc, argv);
    ASSERT_STR("-h", result.opt);
    ASSERT_NULL(result.dir);
}

CTEST(argv_parse, check_option_and_path)
{
    char* argv[] = {"./dse", "-a", "/home"};
    int argc = 3;
    argv_t result = argv_parse(argc, argv);
    ASSERT_STR("-a", result.opt);
    ASSERT_STR("/home", result.dir);
}

CTEST(argv_parse, check_only_path)
{
    char* argv[] = {"./dse", "/home"};
    int argc = 2;
    argv_t result = argv_parse(argc, argv);
    ASSERT_NULL(result.opt);
    ASSERT_STR("/home", result.dir);
}

CTEST(byte_converter, check_convert_to_bytes)
{
    size_t byte = 0;
    for (size_t i = 0; i < KB; i++, byte++)
        ASSERT_EQUAL(i, byte_converter(byte));
}

CTEST(byte_converter, check_convert_to_KB)
{
    size_t kb1 = KB;
    size_t kb2 = MB - KB;
    ASSERT_EQUAL(1, byte_converter(kb1));
    ASSERT_EQUAL(1023, byte_converter(kb2));
}

CTEST(byte_converter, check_convert_to_MB)
{
    size_t mb1 = MB;
    size_t mb2 = GB - MB;
    ASSERT_EQUAL(1, byte_converter(mb1));
    ASSERT_EQUAL(1023, byte_converter(mb2));
}

CTEST(byte_converter, check_convert_to_GB)
{
    size_t gb1 = GB;
    size_t gb2 = (long)GB * (long)5;
    const size_t expect_gb2 = gb2 / GB;
    ASSERT_EQUAL(1, byte_converter(gb1));
    ASSERT_EQUAL(expect_gb2, byte_converter(gb2));
}

CTEST(type_converter, check_convert_to_byte_type)
{
    const char expect_type = 'b';
    for (size_t byte = 0; byte < KB; byte++)
        ASSERT_EQUAL(expect_type, type_converter(byte));
}

CTEST(type_converter, check_convert_to_kb_type)
{
    const char expect_type = 'K';
    size_t kb1 = KB;
    size_t kb2 = MB - KB;
    ASSERT_EQUAL(expect_type, type_converter(kb1));
    ASSERT_EQUAL(expect_type, type_converter(kb2));
}

CTEST(type_converter, check_convert_to_mb_type)
{
    const char expect_type = 'M';
    size_t mb1 = MB;
    size_t mb2 = GB - MB;
    ASSERT_EQUAL(expect_type, type_converter(mb1));
    ASSERT_EQUAL(expect_type, type_converter(mb2));
}

CTEST(type_converter, check_convert_to_GB_type)
{
    const char expect_type = 'G';
    size_t gb1 = GB;
    size_t gb2 = (long)GB * (long)5;
    ASSERT_EQUAL(expect_type, type_converter(gb1));
    ASSERT_EQUAL(expect_type, type_converter(gb2));
}

CTEST(converter, check_converter)
{
    const size_t expect_size = 3;
    const char expect_type = 'K';

    Listnode* node = listnode_create("testnode", 8);
    node->byte = 3 * KB;

    size_t actual_byte = converter(node);

    ASSERT_EQUAL(expect_size, actual_byte);
    ASSERT_EQUAL(expect_type, node->size_type);
}

CTEST(listnode_create, check_listnode_create)
{
    Listnode* node = listnode_create("testnode", 8);

    const char* expect_name = "testnode";
    const char expect_type = 8;
    const char expect_size_type = 'b';

    ASSERT_EQUAL(*expect_name, *node->name);
    ASSERT_EQUAL(expect_type, node->type);
    ASSERT_EQUAL(expect_size_type, node->size_type);
    ASSERT_NULL(node->next);
    ASSERT_NULL(node->prev);
}

CTEST(listnode_add, check_listnode_add)
{
    Listnode* node = listnode_create("testnode", 8);
    Listnode* newnode = listnode_add(node, "testadd", 8);

    const char* expect_name = "testadd";
    const char expect_type = 8;
    const char expect_size_type = 'b';

    ASSERT_EQUAL(*expect_name, *newnode->next->name);
    ASSERT_EQUAL(expect_type, newnode->next->type);
    ASSERT_EQUAL(expect_size_type, newnode->next->size_type);
    ASSERT_NOT_NULL(newnode->next);
    ASSERT_NOT_NULL(newnode->next->prev);
}

CTEST(listdir_create, check_lisdir_create)
{
    char* path = "testpath";
    Listdir* ldir = listdir_create(path);

    const char expect_size_type = 'b';

    ASSERT_STR(path, ldir->path_dir);
    ASSERT_EQUAL(expect_size_type, ldir->size_type);
    ASSERT_NULL(ldir->node);
    ASSERT_NULL(ldir->next);
    ASSERT_NULL(ldir->prev);
}

CTEST(listdir_add, check_listdir_add)
{
    char* path1 = "testpath1";
    char* path2 = "testpath2";

    Listdir* ldir = listdir_create(path1);
    Listdir* newdir = listdir_create(path2);

    listdir_add(ldir, newdir);

    ASSERT_NOT_NULL(ldir->next);
    ASSERT_NOT_NULL(ldir->next->prev);
}

CTEST(change_path, check_change_path)
{
    char* path = "/home/user";
    char* name = "testdir";
    const char* expect_mod_path = "/home/user/testdir";

    char* mod_path = change_path(path, name);
    ASSERT_NOT_NULL(mod_path);
    ASSERT_STR(expect_mod_path, mod_path);
}

CTEST(absolute_root_path, check_valid_path)
{
    char* path = NULL;
    const int result = absolute_root_path(&path);
    ASSERT_EQUAL(0, result);
    ASSERT_NOT_NULL(path);
    ASSERT_TRUE(path[0] == '/');
}

CTEST(fill_listnode, check_filling_node)
{
    char* path = "/tmp/testdir";
    Listdir* ldir = listdir_create(path);

    mkdir(path, 0755);
    FILE* fp1 = fopen("/tmp/testdir/file1.txt", "w");
    fclose(fp1);
    FILE* fp2 = fopen("/tmp/testdir/file2.txt", "w");
    fclose(fp2);
    mkdir("/tmp/testdir/subdir", 0755);

    DIR* dir = opendir(path);
    ASSERT_NOT_NULL(dir);

    int result = fill_listnode(dir, ldir, 0);
    ASSERT_EQUAL(0, result);

    closedir(dir);
    rmdir("/tmp/testdir/subdir");
    unlink("/tmp/testdir/file1.txt");
    unlink("/tmp/testdir/file2.txt");
    rmdir("/tmp/testdir");
}

CTEST(count_bytes_dir, check_count_bytes_dir)
{
    char* path = "/testdir";
    char* name1 = "test1.txt";
    char* name2 = "test2.txt";
    Listdir* ldir = listdir_create(path);
    ldir->node = listnode_add(ldir->node, name1, 8);
    ldir->node = listnode_add(ldir->node, name2, 8);
    ldir->node->byte = 100;
    ldir->node->next->byte = 200;

    int result = count_bytes_dir(ldir);
    ASSERT_EQUAL(0, result);

    ASSERT_EQUAL(300, ldir->byte_dir);
    ASSERT_EQUAL(100, ldir->node->byte);
    ASSERT_EQUAL(200, ldir->node->next->byte);

    listdir_free(ldir);
}

CTEST(swap, check_swap_node)
{
    Listnode* node1 = listnode_create("test1.txt", 8);
    node1->byte = 100;

    Listnode* node2 = listnode_create("test2.txt", 8);
    node2->byte = 200;

    const size_t expect_node1_byte = 100;
    const size_t expect_node2_byte = 200;

    swap(node1, node2);

    ASSERT_EQUAL(expect_node2_byte, node1->byte);
    ASSERT_EQUAL(expect_node1_byte, node2->byte);
}

CTEST(sort_items, check_sort_files)
{
    Listnode* node1 = listnode_create("test1.txt", 8);
    Listnode* node2 = listnode_add(node1, "test2.txt", 8);
    Listnode* node3 = listnode_add(node2, "test3.txt", 8);

    node3->byte = 50;
    node3->next->byte = 100;
    node3->next->next->byte = 75;

    sort_items(node3, 8);

    ASSERT_EQUAL(100, node3->byte);
    ASSERT_EQUAL(50, node3->next->byte);
    ASSERT_EQUAL(75, node3->next->next->byte);
}

CTEST(sort_items_listnode, check_sort_dir)
{
    char* path = "/tmp/testdir";
    Listdir* ldir = listdir_create(path);

    ldir->node = listnode_add(ldir->node, "test1.txt", 8);
    ldir->node->byte = 100;

    ldir->node = listnode_add(ldir->node, "test2.txt", 8);
    ldir->node->byte = 150;

    ldir->node = listnode_add(ldir->node, "test3.txt", 8);
    ldir->node->byte = 50;

    sort_items_listnode(ldir);

    ASSERT_EQUAL(150, ldir->node->byte);
    ASSERT_EQUAL(100, ldir->node->next->byte);
    ASSERT_EQUAL(50, ldir->node->next->next->byte);
}

CTEST(fill_listdir, check_filling_dir)
{
    char* path = "/tmp/testdir";
    argv_t argv = {0, 0};
    Listdir* ldir = listdir_create(path);
    ASSERT_NOT_NULL(ldir);

    const int result = fill_listdir(ldir, 0, argv);
    ASSERT_EQUAL(0, result);
}
