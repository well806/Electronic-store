#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

struct Sale {
    int sale_id;
    string date;
    int product_id;
    int quantity;
    double price;
};

struct Product {
    int product_id;
    string product_name;
    string category;
    float price;
};

struct Customer {
    int customer_id;
    string customer_name;
    string region;
};

struct Time {
    string sale_date;
    int year;
    int quarter;
    int month;
    int day;
    string day_of_week;
};

vector<Sale> readSalesCSV(const string& filename) {
    vector<Sale> sales;
    ifstream file(filename);
    string line;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        Sale sale;
        string date;

        getline(ss, date, ',');
        sale.date = date;
        ss >> sale.product_id;
        ss.ignore(1);
        ss >> sale.quantity;
        ss.ignore(1);
        ss >> sale.price;

        sales.push_back(sale);
    }

    return sales;
}

vector<Product> readProductsCSV(const string& filename) {
    vector<Product> products;
    ifstream file(filename);
    string line;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        Product product;
        string price;

        ss >> product.product_id;
        ss.ignore(1);
        getline(ss, product.product_name, ',');
        getline(ss, product.category, ',');
        getline(ss, price, ',');
        product.price = stof(price);

        products.push_back(product);
    }

    return products;
}

vector<Customer> readCustomersCSV(const string& filename) {
    vector<Customer> customers;
    ifstream file(filename);
    string line;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        Customer customer;

        ss >> customer.customer_id;
        ss.ignore(1);
        getline(ss, customer.customer_name, ',');
        getline(ss, customer.region, ',');

        customers.push_back(customer);
    }

    return customers;
}

vector<Time> readTimeCSV(const string& filename) {
    vector<Time> times;
    ifstream file(filename);
    string line;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        Time time;
        string date;

        getline(ss, date, ',');
        time.sale_date = date;
        ss >> time.year;
        ss.ignore(1);
        ss >> time.quarter;
        ss.ignore(1); 
        ss >> time.month;
        ss.ignore(1); 
        ss >> time.day;
        ss.ignore(1);
        getline(ss, time.day_of_week, ',');

        times.push_back(time);
    }

    return times;
}

void insertSalesToDatabase(const vector<Sale>& sales) {
    try {
        connection C("dbname=dwh user=postgres password=08062006");
        work W(C);

        for (const auto& sale : sales) {
            string query = "INSERT INTO sales_fact (sale_date, product_id, quantity, price) VALUES ('" +
                sale.date + "', " + to_string(sale.product_id) + ", " +
                to_string(sale.quantity) + ", " + to_string(sale.price) + ");";
            cout << query << endl; 
            W.exec(query);
        }

        W.commit();
        cout << "Sales data inserted successfully." << endl;

    }
    catch (const std::exception& e) {
        cerr << "Database error (Sales): " << e.what() << endl;
    }
}
void insertProductsToDatabase(const vector<Product>& products) {
    try {
        connection C("dbname=dwh user=postgres password=08062006");
        work W(C);

        for (const auto& product : products) {
            string query = "INSERT INTO products_dim (product_id, product_name, category, price) VALUES (" +
                to_string(product.product_id) + ", '" + product.product_name + "', '" + product.category + "', " +
                to_string(product.price) + ");";
            cout << query << endl; 
            W.exec(query);
        }

        W.commit();
        cout << "Products data inserted successfully." << endl;

    }
    catch (const std::exception& e) {
        cerr << "Database error (Products): " << e.what() << endl;
    }
}

void insertCustomersToDatabase(const vector<Customer>& customers) {
    try {
        connection C("dbname=dwh user=postgres password=08062006");
        work W(C);

        for (const auto& customer : customers) {
            string query = "INSERT INTO customers_dim (customer_id, customer_name, region) VALUES (" +
                to_string(customer.customer_id) + ", '" + customer.customer_name + "', '" + customer.region + "');";
            cout << query << endl; 
            W.exec(query);
        }

        W.commit();
        cout << "Customers data inserted successfully." << endl;

    }
    catch (const std::exception& e) {
        cerr << "Database error (Customers): " << e.what() << endl;
    }
}

void insertTimeToDatabase(const vector<Time>& sales) {
    try {
        connection C("dbname=dwh user=postgres password=08062006");
        work W(C);

        for (const auto& time : sales) {
            string query = "INSERT INTO time_dim (sale_date, year, quarter, month, day, day_of_week) VALUES ('" +
                time.sale_date + "', " + to_string(time.year) + ", " + to_string(time.quarter) + ", " +
                to_string(time.month) + ", " + to_string(time.day) + ", '" + time.day_of_week + "');";
            cout << query << endl; 
            W.exec(query);
        }

        W.commit();
        cout << "Time data inserted successfully." << endl;

    }
    catch (const std::exception& e) {
        cerr << "Database error (Time): " << e.what() << endl;
    }
}

int main() {
    string salesFilename = "sales_data.csv";
    string productsFilename = "products.csv";
    string customersFilename = "customers.csv";
    string timeFilename = "time.csv";

    vector<Sale> sales = readSalesCSV(salesFilename);
    vector<Product> products = readProductsCSV(productsFilename);
    vector<Customer> customers = readCustomersCSV(customersFilename);
    vector<Time> times = readTimeCSV(timeFilename);

    insertSalesToDatabase(sales);
    insertProductsToDatabase(products);
    insertCustomersToDatabase(customers);
    insertTimeToDatabase(times);

    return 0;
}