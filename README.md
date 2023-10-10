# Shopping_Terminal

## Approach:
The problem has been solved using the concepts of socket programming for communication between client (customer/buyer) and server (Seller/Admin). The Server side holds the inventory of the products to be sold (which is stored in a file). We implemented concurrent socket using fork system call to allow multiple clients to make requests to the server simultaneously. Further for concurrency control in buying, updating, deleting and adding of products we have implemented appropriate file locking mechanism. Any updates or purchases in the Products Inventory get appended to the 'markets.log' file and also a receipt file is generated at the client side on successfull transaction.


## Users:
- **Admin:** The admin can update the Products Inventory with update, delete and add options. File locking has been implemented for concurrency issues if multiple admins try to update some record. The admin doesn't communicate through socket programming because it holds the product resources. The socket communication is for the client to communicate with the remote resources(Product Inventory) held by the admin.
-**Client:** The client can send requests for View Cart , Add Item and Purchase Cart through socket to the server and get appropriate responses after the appropriate operations have been done on the server side. Here also file locking has been implemented on the server for concurrency control of multiple clients accessing the same resources.


## The Project consists of the following files:
- **admin_server.c:** This file contains the server side of the socket programming.
- **admin.c:** This is the file which contains operations for the admin to manipulate the Products Inventory.
- **client.c:** This contains the client side of the socket programming, this contains the code for the client to interact with the server.
- **markets.log:** Contains the log of any update,delete,add or successfull purchase happening in the market with the time stamp.
- **products:** Contains the 'struct item' of every product in the market. To be read using 'read.c' file
- **receipt_XIDX.log:** This is the receipt file generated on successfull purchase on the client side where 'XIDX' is the recipt ID which will be different for every purchase


## Execution:
* Compile and execute the 'admin_server.c' file. If the bind is successfull the the server will be ready to accept connections. If not then an error message is returned and the program exists.
* Run the 'admin.c' file , it will present a menu to update , delete and add items to the inventory. 
* Run the 'client.c' file, it will present a menu to view cart, add items to cart , view products available and buy cart options. On a successfull purchase a receipt file would be generated.
* Run the "read.c" file to view the 'products' file containg the list of products.
* Example receipt files are also attached.
