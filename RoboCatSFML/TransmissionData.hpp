/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

class DeliveryNotificationManager;

class TransmissionData
{
public:
	virtual void HandleDeliveryFailure(DeliveryNotificationManager* inDeliveryNotificationManager) const = 0;
	virtual void HandleDeliverySuccess(DeliveryNotificationManager* inDeliveryNotificationManager) const = 0;
};
typedef shared_ptr< TransmissionData > TransmissionDataPtr;
