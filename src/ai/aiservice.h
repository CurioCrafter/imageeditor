#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <memory>
#include <functional>

namespace core {
class Document;
class Layer;
class Selection;
}

namespace ai {

class AIModel;
class AITask;
class AITileProcessor;

/**
 * @brief AI service integration for advanced image processing
 * 
 * The AIService provides:
 * - Communication with AI microservices
 * - Model management and versioning
 * - Tile-based processing for large images
 * - Progress tracking and cancellation
 * - Local fallback when services unavailable
 * - Batch processing and queuing
 */
class AIService : public QObject
{
    Q_OBJECT

public:
    enum ServiceType {
        Segmentation,       // Object/background segmentation
        Inpainting,        // Content-aware fill and removal
        Upscaling,         // Super-resolution and enhancement
        StyleTransfer,     // Artistic style application
        ColorHarmonization, // Color matching and harmonization
        Relighting,        // Lighting and shadow adjustment
        Denoising,         // Noise reduction and artifact removal
        FaceRestoration,   // Face enhancement and repair
        TextRecognition,   // OCR and text vectorization
        ContentAware       // Content-aware scaling and warping
    };
    
    enum ProcessingMode {
        Local,             // Local GPU inference
        Remote,            // Remote microservice
        Hybrid,            // Local + remote fallback
        Offline            // No AI processing available
    };

    AIService(QObject* parent = nullptr);
    ~AIService();

    // Service management
    bool initialize();
    void shutdown();
    
    bool isAvailable() const { return m_available; }
    ProcessingMode processingMode() const { return m_processingMode; }
    
    // Model management
    QStringList availableModels(ServiceType type) const;
    QString currentModel(ServiceType type) const;
    bool setModel(ServiceType type, const QString& modelName);
    
    QStringList availableVersions(const QString& modelName) const;
    QString currentVersion(const QString& modelName) const;
    bool setModelVersion(const QString& modelName, const QString& version);
    
    // Task processing
    QString processImage(ServiceType type, 
                        const QImage& input,
                        const QJsonObject& parameters = QJsonObject());
    
    QString processSelection(ServiceType type,
                           core::Document* document,
                           const core::Selection* selection,
                           const QJsonObject& parameters = QJsonObject());
    
    QString processLayer(ServiceType type,
                        core::Layer* layer,
                        const QJsonObject& parameters = QJsonObject());
    
    // Batch processing
    QString processBatch(ServiceType type,
                        const QList<QImage>& images,
                        const QJsonObject& parameters = QJsonObject());
    
    // Progress and status
    bool isTaskRunning(const QString& taskId) const;
    double taskProgress(const QString& taskId) const;
    QString taskStatus(const QString& taskId) const;
    QString taskError(const QString& taskId) const;
    
    void cancelTask(const QString& taskId);
    void cancelAllTasks();
    
    // Configuration
    void setServiceEndpoint(const QString& endpoint);
    QString serviceEndpoint() const { return m_serviceEndpoint; }
    
    void setApiKey(const QString& apiKey);
    QString apiKey() const { return m_apiKey; }
    
    void setTimeout(int timeoutMs);
    int timeout() const { return m_timeout; }
    
    void setMaxConcurrentTasks(int maxTasks);
    int maxConcurrentTasks() const { return m_maxConcurrentTasks; }
    
    // Local processing
    void enableLocalProcessing(bool enable);
    bool isLocalProcessingEnabled() const { return m_localProcessingEnabled; }
    
    void setLocalModelPath(const QString& path);
    QString localModelPath() const { return m_localModelPath; }
    
    // Performance monitoring
    double averageProcessingTime(ServiceType type) const;
    double successRate(ServiceType type) const;
    QStringList recentErrors() const;
    
    // Model registry
    void refreshModelRegistry();
    void downloadModel(const QString& modelName, const QString& version);
    void removeModel(const QString& modelName, const QString& version);
    
    // Safety and content filtering
    void enableContentFiltering(bool enable);
    bool isContentFilteringEnabled() const { return m_contentFilteringEnabled; }
    
    void setSafetyLevel(int level); // 0-100
    int safetyLevel() const { return m_safetyLevel; }

signals:
    void taskStarted(const QString& taskId, ServiceType type);
    void taskProgressUpdated(const QString& taskId, double progress);
    void taskCompleted(const QString& taskId, const QImage& result);
    void taskFailed(const QString& taskId, const QString& error);
    void taskCancelled(const QString& taskId);
    
    void modelChanged(ServiceType type, const QString& modelName);
    void modelVersionChanged(const QString& modelName, const QString& version);
    void modelRegistryUpdated();
    
    void serviceStatusChanged(bool available);
    void processingModeChanged(ProcessingMode mode);
    
    void localProcessingEnabled(bool enabled);
    void contentFilteringChanged(bool enabled);
    void safetyLevelChanged(int level);

private slots:
    void onNetworkReplyFinished();
    void onNetworkReplyError(QNetworkReply::NetworkError error);
    void onLocalProcessingComplete(const QString& taskId, const QImage& result);
    void onLocalProcessingError(const QString& taskId, const QString& error);

private:
    // Service state
    bool m_available;
    ProcessingMode m_processingMode;
    QString m_serviceEndpoint;
    QString m_apiKey;
    int m_timeout;
    int m_maxConcurrentTasks;
    
    // Local processing
    bool m_localProcessingEnabled;
    QString m_localModelPath;
    
    // Content safety
    bool m_contentFilteringEnabled;
    int m_safetyLevel;
    
    // Network management
    QNetworkAccessManager* m_networkManager;
    QMap<QNetworkReply*, QString> m_activeReplies;
    
    // Task management
    QMap<QString, AITask*> m_activeTasks;
    QQueue<AITask*> m_taskQueue;
    
    // Model registry
    QMap<ServiceType, QStringList> m_availableModels;
    QMap<ServiceType, QString> m_currentModels;
    QMap<QString, QStringList> m_modelVersions;
    QMap<QString, QString> m_currentVersions;
    
    // Performance metrics
    QMap<ServiceType, QList<double>> m_processingTimes;
    QMap<ServiceType, int> m_successCounts;
    QMap<ServiceType, int> m_totalCounts;
    QStringList m_recentErrors;
    
    // Local processing
    AITileProcessor* m_tileProcessor;
    QMap<QString, std::unique_ptr<AIModel>> m_localModels;
    
    // Internal methods
    QString generateTaskId() const;
    AITask* createTask(ServiceType type, const QJsonObject& parameters);
    void queueTask(AITask* task);
    void processNextTask();
    
    bool validateParameters(ServiceType type, const QJsonObject& parameters);
    QJsonObject prepareRequest(ServiceType type, const QImage& input, 
                              const QJsonObject& parameters);
    
    void updateModelRegistry();
    void loadLocalModels();
    bool downloadModelFile(const QString& modelName, const QString& version);
    
    void updatePerformanceMetrics(ServiceType type, double processingTime, bool success);
    void logError(const QString& error);
    
    // Network helpers
    QNetworkReply* sendRequest(const QUrl& url, const QJsonObject& data);
    QJsonObject parseResponse(const QByteArray& response);
    bool handleNetworkError(QNetworkReply* reply);
    
    // Content filtering
    bool filterContent(const QImage& input, const QJsonObject& parameters);
    bool validatePrompt(const QString& prompt);
    bool validateOutput(const QImage& output);
};

} // namespace ai
