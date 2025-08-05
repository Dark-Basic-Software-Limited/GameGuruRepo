-- LUA Script - precede every function and global member with lowercase name of script + '_main'
-- Player Collects Key

keyname = {}

function key_init(e)
end

function key_init_name(e, name)
keyname[e] = name
end

function key_main(e)
 PlayerDist = GetPlayerDistance(e)
 
 -- If in first person collect key only if within 150 distance from the player and if the player is looking in the direction of the key
 if PlayerDist < 150 and g_PlayerHealth > 0 and g_PlayerThirdPerson == 0 then
	SourceAngle = g_PlayerAngY
	while SourceAngle < 0.0 do
		SourceAngle = SourceAngle + 360.0
	end	
	while SourceAngle > 340.0 do
		SourceAngle = SourceAngle - 360.0
	end
		
    PlayerDX = (g_Entity[e]['x'] - g_PlayerPosX)
    PlayerDZ = (g_Entity[e]['z'] - g_PlayerPosZ)
	DestAngle = math.atan2( PlayerDZ , PlayerDX )
	-- Convert to degrees
	DestAngle = (DestAngle * 57.2957795) - 90.0
	
	Result = math.abs(math.abs(SourceAngle)-math.abs(DestAngle))
	if Result > 180 then
		Result = 0
	end	
	
	if Result < 20.0 then
		if IntersectStatic(g_PlayerPosX,g_PlayerPosY+50,g_PlayerPosZ,g_Entity[e]['x'],g_Entity[e]['y'],g_Entity[e]['z'],g_Entity[e]['obj']) == 0 then
			if GetGamePlayerStateXBOX() == 1 then
			 Prompt("Press Y button to pick up "..keyname[e])
			else
			 Prompt("Press E to pick up "..keyname[e])
			end
			if g_KeyPressE == 1 then
				Prompt("Collected "..keyname[e])
				PlaySound(e,0)
				Collected(e)
				Destroy(e)
				ActivateIfUsed(e)
			end
		end
   end
 end
 
	-- If third person then automatically collect key when near
	if PlayerDist < 80 and g_PlayerHealth > 0 and g_PlayerThirdPerson > 0 then
 				Prompt("Collected "..keyname[e])
				PlaySound(e,0)
				Collected(e)
				Destroy(e)
				ActivateIfUsed(e)
	end
 
 
end
