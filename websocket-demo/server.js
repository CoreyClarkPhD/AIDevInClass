const WebSocket = require('ws');
const { v4: uuidv4 } = require('uuid');

class WebSocketServer {
    constructor(port = 9002) {
        this.port = port;
        this.clients = new Set();
        this.server = null;
    }

    start() {
        this.server = new WebSocket.Server({ port: this.port });

        this.server.on('listening', () => {
            console.log(`WebSocket server started on port ${this.port}`);
        });

        this.server.on('connection', (ws) => {
            console.log('New client connected');
            this.clients.add(ws);

            // Set up message handler for this client
            ws.on('message', (data) => {
                try {
                    const message = JSON.parse(data);
                    this.handleMessage(ws, message);
                } catch (error) {
                    this.sendError(ws, 'Invalid message format', 1001);
                }
            });

            // Handle client disconnect
            ws.on('close', () => {
                console.log('Client disconnected');
                this.clients.delete(ws);
            });

            // Send welcome message
            this.sendEvent(ws, 'connectionStatus', {
                status: 'connected',
                timestamp: Date.now()
            });
        });
    }

    handleMessage(ws, message) {
        if (!this.validateMessage(message)) {
            this.sendError(ws, 'Missing required message fields', 1002);
            return;
        }

        console.log('Received message:', message);

        switch (message.type) {
            case 'command':
                this.handleCommand(ws, message);
                break;
            case 'event':
                this.handleEvent(ws, message);
                break;
            case 'response':
                this.handleResponse(ws, message);
                break;
            default:
                this.sendError(ws, 'Invalid message type', 1003);
        }
    }

    handleCommand(ws, message) {
        const command = message.payload.command;
        console.log('Handling command:', command.name);

        // Example command handling
        switch (command.name) {
            case 'getData':
                // Simulate getting data
                this.sendResponse(ws, message.id, {
                    data: {
                        temperature: Math.random() * 30,
                        humidity: Math.random() * 100
                    }
                });
                break;
            default:
                this.sendError(ws, 'Unknown command', 2001);
        }
    }

    handleEvent(ws, message) {
        const event = message.payload.event;
        console.log('Handling event:', event.name);
        // Echo back the event to all other clients
        this.broadcast(message, ws);
    }

    handleResponse(ws, message) {
        console.log('Handling response:', message);
        // Handle response logic here
    }

    sendMessage(ws, message) {
        if (ws.readyState === WebSocket.OPEN) {
            ws.send(JSON.stringify(message));
        }
    }

    sendEvent(ws, eventName, data) {
        const message = {
            type: 'event',
            id: uuidv4(),
            timestamp: Date.now(),
            payload: {
                event: {
                    name: eventName,
                    data: data
                }
            }
        };
        this.sendMessage(ws, message);
    }

    sendResponse(ws, requestId, data) {
        const message = {
            type: 'response',
            id: uuidv4(),
            timestamp: Date.now(),
            payload: {
                response: {
                    success: true,
                    data: data
                }
            }
        };
        this.sendMessage(ws, message);
    }

    sendError(ws, errorMessage, errorCode) {
        const message = {
            type: 'error',
            id: uuidv4(),
            timestamp: Date.now(),
            payload: {
                error: {
                    code: errorCode,
                    message: errorMessage
                }
            }
        };
        this.sendMessage(ws, message);
    }

    broadcast(message, exclude = null) {
        this.clients.forEach(client => {
            if (client !== exclude && client.readyState === WebSocket.OPEN) {
                client.send(JSON.stringify(message));
            }
        });
    }

    validateMessage(message) {
        return message.type &&
               message.id &&
               message.timestamp &&
               message.payload;
    }

    stop() {
        if (this.server) {
            this.server.close(() => {
                console.log('Server stopped');
            });
        }
    }
}

// Create and start the server
const server = new WebSocketServer();
server.start();

// Handle process termination
process.on('SIGINT', () => {
    console.log('Shutting down server...');
    server.stop();
    process.exit();
});