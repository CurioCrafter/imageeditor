#!/usr/bin/env python3
"""
Collaboration Service for Advanced Image Editor
"""

import os
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI(title="Collaboration Service", version="1.0.0")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/health")
async def health_check():
    return {"status": "healthy", "service": "collaboration"}

@app.get("/")
async def root():
    return {"service": "Collaboration Service", "version": "1.0.0"}

if __name__ == "__main__":
    import uvicorn
    host = os.getenv("HOST", "0.0.0.0")
    port = int(os.getenv("PORT", "8082"))
    uvicorn.run(app, host=host, port=port)
