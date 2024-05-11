#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

using namespace std;

// Function to execute SQL queries
int executeQuery(sqlite3* db, string query) {
    char* errorMessage;
    int rc = sqlite3_exec(db, query.c_str(), NULL, 0, &errorMessage);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return 1;
    }
    return 0;
}

// Function to initialize SQLite database
void initializeDatabase(sqlite3* db) {
    string createProductTable = "CREATE TABLE IF NOT EXISTS products ("
                                "id INTEGER PRIMARY KEY,"
                                "name TEXT NOT NULL,"
                                "category TEXT NOT NULL,"
                                "price REAL NOT NULL,"
                                "quantity INTEGER NOT NULL"
                                ");";

    string createSalesTable = "CREATE TABLE IF NOT EXISTS sales ("
                              "id INTEGER PRIMARY KEY,"
                              "product_id INTEGER,"
                              "quantity INTEGER,"
                              "amount REAL,"
                              "sold_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                              "FOREIGN KEY (product_id) REFERENCES products(id)"
                              ");";

    if (executeQuery(db, createProductTable) || executeQuery(db, createSalesTable)) {
        cerr << "Error initializing database." << endl;
        sqlite3_close(db);
        exit(1);
    } else {
        cout << "Database initialized successfully." << endl;
    }
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("inventory.db", &db);
    if (rc) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    initializeDatabase(db);
    cout << "Inventory Management System Initialized." << endl;

    // Sample SQL queries for inserting data
    string insertProductQuery = "INSERT INTO products (name, category, price, quantity) "
                                "VALUES ('Laptop', 'Electronics', 999.99, 10);";

    string insertSalesQuery = "INSERT INTO sales (product_id, quantity, amount) "
                              "VALUES (1, 5, 4999.95);";

    // Execute sample SQL queries
    executeQuery(db, insertProductQuery);
    executeQuery(db, insertSalesQuery);

    // Perform other operations as needed

    sqlite3_close(db);
    return 0;
}
