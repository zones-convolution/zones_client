import { useLocation, useNavigate, Navigate } from "react-router-dom";

import { ZoneMetadata } from "@/hooks/zone_metadata";

const Zone = () => {
  const { state } = useLocation();

  try {
    let userZone = ZoneMetadata.parse(state);

    return (
      <div className="bg-card">
        <h1 className="text-xl">{userZone.title}</h1>
      </div>
    );
  } catch (err) {
    return <Navigate to="/" replace />;
  }
};

export default Zone;
