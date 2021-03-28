////////////////////////////////////////////////////////////////////////////////
//
// SQLite Tutorial
// https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "jansson.h"
/* Add your header comment here */
// #include <sqlite3ext.h> /* Do not use <sqlite3.h>! */

connect();
create_table();
insert();
select();
update();
delete ();
json();

int main(int argc, char *argv[])
{
  // connect();
  // create_table();
  // insert();
  // select();
  // update();
  // delete ();

  json();
  return 0;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  int i;
  for (i = 0; i < argc; i++)
  {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

static int callback_json(void *NotUsed, int argc, char **argv, char **azColName)
{
  json_t *root;
  json_error_t error;
  for (int i = 0; i < argc; i++)
  {
    if (strcmp(azColName[i], "JSON") == 0)
    {
      root = json_loads(argv[i], 0, &error);
      if (!root)
      {
        fprintf(stderr, "Error: on line %d: %s\n", error.line, error.text);
      }
      else
        printf("%s\n", argv[i]);
      if (!json_is_object(root))
      {
        fprintf(stderr, "Error: string is not an json array\n");
        json_decref(root);
      }
      else
      {
        const char *key;
        json_t *value;

        json_object_foreach(root, key, value)
        {
          printf("%s\n", key);
          if (json_is_integer(value))
            printf("%d\n", json_integer_value(value));
          else if (json_is_real(value))
            printf("%f\n", json_real_value(value));
          // switch (json_typeof(value))
          // {
          // case JSON_INTEGER:
          //   printf("%" JSON_INTEGER_FORMAT "\n", value);
          // case JSON_REAL:
          //   printf("%f\n", json_real_value(value));
          // default:
          //   printf("Undetermined.\n");
          // }
        }
      }
    }
  }
  printf("\n");
  return 0;
}

json()
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql;

  rc = sqlite3_open("test.db", &db);
  int retval = 0;
  if (rc)
  {
    fprintf(stdout, "Can't open database: %s\n", sqlite3_errmsg(db));
    retval = 1;
  }
  else
  {
    fprintf(stdout, "Opened database successfully\n");
  }

  // rc = sqlite3_load_extension(db, "json1.dll", 0, &zErrMsg);
  // if (rc != SQLITE_OK)
  // {
  //   fprintf(stderr, "SQL error: %s\n", zErrMsg);
  //   sqlite3_free(zErrMsg);
  //   retval = 1;
  // }
  // else
  // {
  //   fprintf(stdout, "Extnesion loaded successfully\n");
  // }

  /* Create SQL statement */
  sql = "CREATE TABLE IF NOT EXISTS JSONS("
        "ID INT PRIMARY KEY     NOT NULL,"
        "JSON           TEXT    NOT NULL);";

  /* Execute SQL statement */
  sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    retval = 1;
  }
  else
  {
    fprintf(stdout, "Table created successfully\n");
  }

  /* Insert JSON item */
  sql = "INSERT INTO JSONS (ID,JSON) "
        "VALUES (3, '{\"id\":1}');"
        "INSERT INTO JSONS (ID,JSON) "
        "VALUES (2, '{\"id\":1.2}');";
  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    retval = 1;
  }
  else
  {
    fprintf(stdout, "JSON inserted successfully\n");
  }

  /* Select JSON item */
  sql = "SELECT * from JSONS";
  rc = sqlite3_exec(db, sql, callback_json, 0, &zErrMsg);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    retval = 1;
  }
  else
  {
    fprintf(stdout, "JSON fetched successfully\n");
  }

  sqlite3_close(db);
}

connect()
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_open("test.db", &db);
  int retval = 0;
  if (rc)
  {
    fprintf(stdout, "Can't open database: %s\n", sqlite3_errmsg(db));
    retval = 1;
  }
  else
  {
    fprintf(stdout, "Opened database successfully\n");
  }
  sqlite3_close(db);
  return retval;
}

create_table()
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql;

  /* Open database */
  rc = sqlite3_open("test.db", &db);

  if (rc)
  {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return (0);
  }
  else
  {
    fprintf(stdout, "Opened database successfully\n");
  }

  /* Create SQL statement */
  sql = "CREATE TABLE IF NOT EXISTS COMPANY("
        "ID INT PRIMARY KEY     NOT NULL,"
        "NAME           TEXT    NOT NULL,"
        "AGE            INT     NOT NULL,"
        "ADDRESS        CHAR(50),"
        "SALARY         REAL );";

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  int retval = 0;

  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    retval = 1;
  }
  else
  {
    fprintf(stdout, "Table created successfully\n");
  }
  sqlite3_close(db);
  return retval;
}

insert()
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql;

  /* Open database */
  rc = sqlite3_open("test.db", &db);

  if (rc)
  {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return (0);
  }
  else
  {
    fprintf(stderr, "Opened database successfully\n");
  }

  /* Create SQL statement */
  sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
        "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
        "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
        "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );"
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
        "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  int retval = 0;

  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    retval = 1;
  }
  else
  {
    fprintf(stdout, "Records created successfully\n");
  }
  sqlite3_close(db);
  return retval;
}

select()
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql;
  const char *data = "Callback function called";

  /* Open database */
  rc = sqlite3_open("test.db", &db);

  if (rc)
  {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return (0);
  }
  else
  {
    fprintf(stderr, "Opened database successfully\n");
  }

  /* Create SQL statement */
  sql = "SELECT * from COMPANY";

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
  int retval = 0;

  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    retval = 1;
  }
  else
  {
    fprintf(stdout, "Operation done successfully\n");
  }
  sqlite3_close(db);
  return retval;
}

update()
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql;
  const char *data = "Callback function called";

  /* Open database */
  rc = sqlite3_open("test.db", &db);

  if (rc)
  {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return (0);
  }
  else
  {
    fprintf(stderr, "Opened database successfully\n");
  }

  /* Create SQL statement */
  sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; "
        "SELECT * from COMPANY";

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
  int retval = 0;

  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    retval = 1;
  }
  else
  {
    fprintf(stdout, "Operation done successfully\n");
  }
  sqlite3_close(db);
  return retval;
}

delete ()
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql;
  const char *data = "Callback function called";

  /* Open database */
  rc = sqlite3_open("test.db", &db);

  if (rc)
  {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return (0);
  }
  else
  {
    fprintf(stderr, "Opened database successfully\n");
  }

  /* Create SQL statement */
  sql = "DELETE from COMPANY where ID=2; "
        "SELECT * from COMPANY";

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
  int retval = 0;

  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    retval = 1;
  }
  else
  {
    fprintf(stdout, "Operation done successfully\n");
  }
  sqlite3_close(db);
  return retval;
}