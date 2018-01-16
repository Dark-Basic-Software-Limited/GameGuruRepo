-- Camera Override Module
g_camera_owner = 0
g_camera_smoothrx = 0
g_camera_smoothry = 0
g_camera_rememberplayerweapon = 0

local module_cameraoverride = {}

function module_cameraoverride.beingattackedby(e,radius)
 haveplrweapid = GetPlayerWeaponID()
 if haveplrweapid > 0 and g_camera_rememberplayerweapon == 0 then
  g_camera_rememberplayerweapon = haveplrweapid
  ChangePlayerWeaponID(0)
 end
 if g_camera_owner == 0 then
  g_camera_owner = e
  SetCameraOverride(3)
  FreezeEntity(e,0)
  RotateToPlayer(e)
  -- would have liked to script hard coordinates but shifts in positions have issues
  --rRX = math.sin(((GetEntityAngleY(e))/360.0)*6.28)*radius
  --rRZ = math.cos(((GetEntityAngleY(e))/360.0)*6.28)*radius
  --tPlayerCamX = g_Entity[e]['x'] + rRX
  --tPlayerCamY = g_Entity[e]['y'] + 60.0
  --tPlayerCamZ = g_Entity[e]['z'] + rRZ
  --SetCameraPosition(0,tPlayerCamX,tPlayerCamY,tPlayerCamZ) 
  g_camera_smoothrx = GetCameraAngleX(0)
  g_camera_smoothry = GetCameraAngleY(0)
 end
end

function module_cameraoverride.hasowner()
 return g_camera_owner
end

function module_cameraoverride.manageattackcycle(e)
 if g_camera_owner == e then
  g_camera_smoothrx = g_camera_smoothrx + ((10-g_camera_smoothrx)*0.01)
  g_camera_smoothry = WrapAngle ( g_camera_smoothry, GetEntityAngleY(e)-165, 0.05 )
  SetCameraAngle(0,g_camera_smoothrx,g_camera_smoothry,0)
  return 1
 else
  return 0
 end
end

function module_cameraoverride.finishbeingattacked(e)
 if g_camera_owner == e then
  if g_camera_rememberplayerweapon > 0 then
   ChangePlayerWeaponID(g_camera_rememberplayerweapon)
   g_camera_rememberplayerweapon = 0
  end
  SetCameraOverride(0)
  UnFreezeEntity(e)
  g_camera_owner = 0
 end
end

return module_cameraoverride
